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


#include <elapsedMillis.h>
#include "ESP_Buttons.h"
#include "mqtt.h"

elapsedMillis timeElapsed;

uint8_t lastEvent = 0;
uint8_t lastLongPress = 0;

void processPressed(uint8_t value, uint8_t buttonValue, int buttonNumber) {
  // Was it already pressed?
  if ((lastEvent & buttonValue) == buttonValue) {
    // Yes it was already pressed
    // Is it a long press?
    if (timeElapsed > LONG_PRESS) {
      if ((lastLongPress & buttonValue) == 0) {
        // new long press
        lastLongPress |= buttonValue;
        Serial.print("L");
        Serial.print(buttonNumber);
        sendButtonMessage(buttonNumber, true);
      } // else Long press, and you're still pressing it
    } // else You're holding the button down, but not long enough for a long press yet
  } else {
    // You're newly pressing a button when it wasn't pressed before
    lastEvent = lastEvent | buttonValue;
    // start the timer to know if it was a long press
    timeElapsed = 0;
  }
}

void processNotPressed(uint8_t value, uint8_t buttonValue, int buttonNumber) {
  // Were we tracking a long press of this button?
  if ((lastLongPress & buttonValue) == 0) {
    // No, but was it previously pressed?
    if ((lastEvent & buttonValue) == buttonValue) {
      // Yes it was. You released after it was previously pressed
      // But was it pressed for long enough?
      if (timeElapsed < SHORT_PRESS) {
        // You didn't press it for long enough. e.g. bounce
        // Forget you ever pressed it
        lastEvent = lastEvent & (~buttonValue);
      } else {
        // short press detected
        Serial.print("S");
        Serial.print(buttonNumber);
        lastEvent = lastEvent & (~buttonValue);
        sendButtonMessage(buttonNumber, false);
      }
    }
  } else {
    // You released it after a long press
    //Serial.println("Release after long");
    lastLongPress = lastLongPress & (~buttonValue);
    lastEvent = lastEvent & (~buttonValue);
  }
}

void processButtonPress(uint8_t value) {
  // The CAP1188 module flips a bit to indicate that a button is being pressed,
  // so examine the bits one at a time, starting with 00000001
  uint8_t buttonValue = 1;
  
  for (int buttonNumber = 0; buttonNumber < NUM_BUTTONS; buttonNumber++) {
    if ((value & buttonValue) == buttonValue) {
      // button pressed
      processPressed(value, buttonValue, buttonNumber);
    } else {
      // button is not pressed
      processNotPressed(value, buttonValue, buttonNumber);
    }

    // Shift one bit to the left to check the next bit
    buttonValue = buttonValue << 1;
  }
}
