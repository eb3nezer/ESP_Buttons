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

#ifndef esp_buttons_h
#define esp_buttons_h

#include <Ticker.h>
#include <PubSubClient.h>

// How many buttons do you care about. The CAP1188 handles up to 8
#define NUM_BUTTONS 8

// How long in ms should you hold a button down before it is considered a short press.
// If you make this too short you may get accidental presses.
#define SHORT_PRESS 10
// How long in ms should you hold a button down before it is considered a long press.
#define LONG_PRESS 1500

extern char mqtt_server[];
extern char mqtt_port[];
extern char mqtt_user[];
extern char mqtt_password[];
extern char mqtt_topic[];
extern char mqtt_device[];

extern Ticker ticker;
extern void tick();

extern PubSubClient mqttClient;

#endif esp_buttons_h

