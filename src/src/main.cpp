#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "credentials.h"
#include "common.h"
#include "mqttClient.h"

void i2c_scanner();

PubSubClient MQTTClient;
MQTT mqtt;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  Wire.begin();

  setupWifi();

  //start MQTT
  mqtt.Begin();

  i2c_scanner();
}

void loop()
{

  delay(50);
}

void i2c_scanner()
{
  yield();

  byte error, address;
  int nDevices;

  Log("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    yield();

    delay(5);

    // The i2c scanner uses the return value of the Write.endTransmisstion to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Log("I2C device found at address 0x");
      if (address < 16)
      {
        Log("0");
      }
      Log(String(address));
      Log(" !");

      nDevices++;
    }
    else if (error == 4)
    {
      Log("Unknown error at address 0x");
      if (address < 16)
      {
        Log("0");
      }
      Log(String(address));
    }
  }
  if (nDevices == 0)
  {
    Log("No I2C devices found\n");

    delay(500);

    ESP.restart();
  }
  else
  {
    Log("Done.\n");
  }
}
