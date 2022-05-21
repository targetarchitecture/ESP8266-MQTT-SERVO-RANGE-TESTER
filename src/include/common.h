#ifndef Common_h

#define Common_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "credentials.h"

extern PubSubClient MQTTClient;

void setupWifi();
void setupOTA();

#endif