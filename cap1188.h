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

#ifndef esp_buttons_cap1188_h
#define esp_buttons_cap1188_h

#include <Arduino.h>

#define CAP1188_DATA_PIN D1
#define CAP1188_CLK_PIN D2

#define CAP1188_I2CADDR 0x29

#define CAP1188_SENINPUTSTATUS 0x3
#define CAP1188_MTBLK 0x2A
#define CAP1188_RECALIBRATE_CONFIG 0x2F
#define CAP1188_LEDLINK 0x72
#define CAP1188_PRODID 0xFD
#define CAP1188_MANUID 0xFE
#define CAP1188_STANDBYCFG 0x41
#define CAP1188_REV 0xFF
#define CAP1188_MAIN 0x00
#define CAP1188_MAIN_INT 0x01
#define CAP1188_LEDPOL 0x73

extern bool setupCap1188(void);
extern uint8_t readCap1188(void);

#endif esp_buttons_cap1188_h
