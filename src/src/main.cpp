#include <Arduino.h>
#include <Wire.h>
#include "EspMQTTClient.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver PCA9685 = Adafruit_PWMServoDriver();

EspMQTTClient client(
    "the robot network",
    "isaacasimov",
    "192.168.1.189", // MQTT Broker server ip
    "public",        // Can be omitted if not needed
    "public",        // Can be omitted if not needed
    "servo-tester",  // Client name that uniquely identify your device
    1883             // The MQTT port, default to 1883. this line can be omitted
);

//i2c pins
#define SDA 27
#define SCL 32

void setup()
{
  
  Wire.begin(SDA, SCL); //I2C bus

  Serial.begin(115200);


  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
                                    // client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  // client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  // client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

  PCA9685.reset();
  PCA9685.setOscillatorFrequency(27000000);
  PCA9685.setPWMFreq(50); // Analog servos run at ~50 Hz updates
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  // client.subscribe("mytopic/test", [](const String & payload) {
  //  Serial.println(payload);
  //});

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("servotest/#", [](const String &topic, const String &payload)
                   {
  Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);

  String temp = topic;
  temp.replace("servotest/","");

  auto pin = atoi(temp.c_str());
  auto pwm = atoi(payload.c_str());

  Serial.print("servo: ");
  Serial.print(pin);
  Serial.print(" PWM: ");
  Serial.println(pwm);
  
  // configure the PWM duty cycle
  PCA9685.setPWM(pin, 0, pwm); 
});

  // Publish a message to "mytopic/test"
  // client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  // client.executeDelayed(5 * 1000, []() {
  //  client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  //});
}

void loop()
{
  client.loop();
}