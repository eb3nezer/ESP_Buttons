/*

This file is heavily based on Adafruit_CAP1188_Library, with some modifications to make
it work with ESP8266.

Adafruit_CAP1188_Library's license:

This is a library for the CAP1188 8-channel Capacitive touch sensor

Designed specifically to work with the CAP1188 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C or SPI to communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution

Modifications Copyright 2017 Ben Kelley.

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

#include <Wire.h>

#include "cap1188.h"

uint8_t i2cReadRegister(uint8_t reg) {
  Wire.beginTransmission(CAP1188_I2CADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(CAP1188_I2CADDR, 1);
  return (Wire.read());
}

void i2cWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(CAP1188_I2CADDR);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(value));
  Wire.endTransmission();
}

bool setupCap1188(void) {
  Serial.println("Configuring i2c");
  Wire.begin(CAP1188_DATA_PIN, CAP1188_CLK_PIN);

  Serial.println("Reading product ID");
  i2cReadRegister(CAP1188_PRODID);
  
  // Debugging info

  Serial.println("Checking CAP1188 Board");
  Serial.print("Product ID: 0x");
  Serial.println(i2cReadRegister(CAP1188_PRODID), HEX);
  Serial.print("Manuf. ID: 0x");
  Serial.println(i2cReadRegister(CAP1188_MANUID), HEX);
  Serial.print("Revision: 0x");
  Serial.println(i2cReadRegister(CAP1188_REV), HEX);

  if ( (i2cReadRegister(CAP1188_PRODID) != 0x50) ||
       (i2cReadRegister(CAP1188_MANUID) != 0x5D) ||
       (i2cReadRegister(CAP1188_REV) != 0x83)) {
    return false;
  }
  
  // disallow multiple touches
  i2cWriteRegister(CAP1188_MTBLK, 0x80); 
  // Have LEDs follow touches
  i2cWriteRegister(CAP1188_LEDLINK, 0xFF);
  // speed up a bit
  i2cWriteRegister(CAP1188_STANDBYCFG, 0x30);
  // recalibration config - recalibrate each input separately
  i2cWriteRegister(CAP1188_RECALIBRATE_CONFIG,  0x0A);

  return true;
}

uint8_t readCap1188(void) {
  uint8_t value = i2cReadRegister(CAP1188_SENINPUTSTATUS);
  if (value) {
    i2cWriteRegister(CAP1188_MAIN, i2cReadRegister(CAP1188_MAIN) & ~CAP1188_MAIN_INT);
  }
  return value;
}
