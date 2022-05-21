#include "mqttClient.h"

MQTT::MQTT()
{
}

void MQTT::Begin()
{
  if (WiFi.isConnected() == true)
  {
    Serial.println("Connecting to MQTT server");

    MQTTClient.setClient(espClient);
    MQTTClient.setServer(MQTT_SERVER, 1883);

    // setup callbacks (https://blog.hobbytronics.pk/arduino-custom-library-and-pubsubclient-call-back/)
    MQTTClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                           { this->callback(topic, payload, length); });

    Serial.println("connect mqtt...");

    String clientId = MQTT_CLIENTID;

    if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.println("Connected");
      MQTTClient.publish(MQTT_LOG_TOPIC, "Connected to MQTT server");

      Serial.println("subscribe");
      MQTTClient.subscribe(MQTT_TOPIC_SUBSCRIBE);
      Serial.println("subscribed");
    }
  }
  else
  {
    Serial.println("Wifi Not Connected");
  }
}

void MQTT::reconnect()
{
  // Loop until we're reconnected
  while (!MQTTClient.connected())
  {
    yield();

    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = MQTT_CLIENTID;

    // Attempt to connect
    if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      MQTTClient.publish(MQTT_LOG_TOPIC, "Reconnected");
      // ... and resubscribe
      MQTTClient.subscribe(MQTT_TOPIC_SUBSCRIBE);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(MQTTClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void MQTT::callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message = "";

  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  Serial.println(message);

  if (std::string(topic) == std::string(MQTT_TOPIC_SUBSCRIBE))
  {

  }
}

void MQTT::publishMQTTmessage(String msg)
{
  MQTTClient.publish(MQTT_LOG_TOPIC, msg.c_str());
}

void MQTT::publishMQTTmessage(String topic, String msg)
{
  MQTTClient.publish(topic.c_str(), msg.c_str());
}

