/*
    Copyright 2017 Ben Kelley.

    This file is part of ESP Buttons.

    ESP Buttons is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ESP Buttons is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ESP Buttons.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FS.h> // Make sure this is first

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <Ticker.h>
#include <Wire.h>
#include <elapsedMillis.h>

#include "config.h"
#include "mqtt.h"
#include "cap1188.h"
#include "button_handler.h"

Ticker ticker;

char mqtt_server[40];
char mqtt_port[6] = "1883";
char mqtt_user[64];
char mqtt_password[64];
char mqtt_topic[64];
char mqtt_device[64];

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void tick() {
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

void setup() {
  Serial.begin(115200);

  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  //use flash button as input
  pinMode(D3, INPUT);

  if (setupCap1188()) {
    Serial.println("Configured CAP1188 board");
  } else {
    Serial.println("WARNING: Failed to configure CAP1188 board");
  }

  configureWifi(true);

  Serial.println("Wifi is now connected");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  configureMQTT();
}

void loop() {
  // Check for user pressing FLASH button, which means reconfigure
  if (digitalRead(D3) == LOW) {
    configureWifi(false);
  }

  // Check if any button pressed
  uint8_t button = readCap1188();
  processButtonPress(button);
  
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  
  if (mqttClient.connected()) {
      mqttClient.loop();
  }
}
