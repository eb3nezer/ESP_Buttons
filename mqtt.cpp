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

#include <Arduino.h>
#include <PubSubClient.h>
#include <elapsedMillis.h>

// If disconnected, how often to try to reconnect
#define RECONNECT_POLLING_PERIOD 5000

elapsedMillis reconnectTimeElapsed;

#include "ESP_Buttons.h"

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void configureMQTT(void) {
  String port(mqtt_port);
  Serial.println("Configuring MQTT");
  Serial.print("Host: ");
  Serial.println(mqtt_server);
  Serial.print("Port: ");
  Serial.print(port.toInt());
  mqttClient.setServer(mqtt_server, port.toInt());
  mqttClient.setCallback(mqttCallback);
}

void reconnectMQTT(void) {
  if (!mqttClient.connected()) {
    if (reconnectTimeElapsed > RECONNECT_POLLING_PERIOD) {
      reconnectTimeElapsed = 0;
      Serial.print("Attempting MQTT connection...");
      Serial.print("Host: ");
      Serial.println(mqtt_server);
      Serial.print("Port: ");
      Serial.println(mqtt_port);
      Serial.print("User: ");
      Serial.println(mqtt_user);
      Serial.println("Password: *****");
      // Attempt to connect
      if (mqttClient.connect(mqtt_device, mqtt_user, mqtt_password)) {
        Serial.println("connected");
        digitalWrite(BUILTIN_LED, LOW);
      } else {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
        // flash the LED for 100ms
        digitalWrite(BUILTIN_LED, LOW);
        delay(100);
        digitalWrite(BUILTIN_LED, HIGH);
      }
    }
  }
}

void sendButtonMessage(int buttonNumber, bool longPress) {
  char topic[30];

  sprintf(topic, "%s/%01d", mqtt_topic, buttonNumber);
  
  Serial.print(" mqttClient.publish(\"");
  Serial.print(topic);

  if (longPress) {
    Serial.println("\", \"long\");");
    mqttClient.publish(topic, "long");
  } else {
    mqttClient.publish(topic, "short");  
    Serial.println("\", \"short\");");
  }
}
