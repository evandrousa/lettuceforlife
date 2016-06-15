/*
  Copyright (c) 2015 Intel Corporation. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-
  1301 USA
*/

int r1 = 16;
int r2 = 5;
int oldLum = 350;
int oldTemp;
int oldHum;
int oldpH = 7;


#include <CurieBLE.h>

const int ledPin = 13; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4

BLEPeripheral blePeripheral; // create peripheral instance
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service


// create switch characteristic and allow remote device to read and write
BLECharCharacteristic r1Characteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic r2Characteristic("19B10012-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite);
BLECharCharacteristic lightCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead); 
BLECharCharacteristic tempCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead);
BLECharCharacteristic humCharacteristic("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead);
BLECharCharacteristic phCharacteristic("19B10016-E8F2-537E-4F6C-D104768A1214", BLERead);

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
  pinMode(buttonPin, INPUT); // use button pin 4 as an input

  // set the local name peripheral advertises
  blePeripheral.setLocalName("Lettuce");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(r1Characteristic);
  blePeripheral.addAttribute(r2Characteristic);
  blePeripheral.addAttribute(lightCharacteristic);
  blePeripheral.addAttribute(tempCharacteristic);
  blePeripheral.addAttribute(humCharacteristic);
  blePeripheral.addAttribute(phCharacteristic);


  r1Characteristic.setValue(r1);
  r2Characteristic.setValue(r2);
    
  // advertise the service
  blePeripheral.begin();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // print a random number from 0 to 299
  int randNumber = random(300);
  Serial.print("R1= ");
  Serial.print(r1Characteristic.value(), HEX);
  Serial.print(" | R2= ");
  Serial.print(r2Characteristic.value(), HEX);
  Serial.print(" | Luminance= ");
  Serial.print(lightCharacteristic.value(), HEX);
  Serial.print(" | Temperature= ");
  Serial.print(tempCharacteristic.value(), HEX);
  Serial.print(" | Humidity = ");
  Serial.print(humCharacteristic.value(), HEX); 
  Serial.print(" | pH= ");
  Serial.print(phCharacteristic.value(), HEX);
  Serial.println(" ");

   
  // poll peripheral
  blePeripheral.poll();

  if (randNumber >150) {
    // button state changed, update characteristics
    updateLuminance();
    updateDHT();
    updatepH();
    } 
  Serial.println(randNumber);
  
  if (r1Characteristic.written()) {
    // update R1, either central has written to characteristic or button state has changed
    r1 = r1Characteristic.value();
    Serial.print("new R1 = ");
    Serial.println(r1);
    if (r1){
      Serial.print("new R2 = ");
      Serial.println(r1);
      r2Characteristic.setValue(r1);
      }
  }
  delay(5000);
}


void updateLuminance() {
   /* Read the current voltage level on the A0 analog input pin.
      This is used here to simulate the heart rate's measurement.
   */
  int luminanceValue = random(1023);
   int lum = map(luminanceValue, 0, 1023, 0, 100);
   if (lum != oldLum) {      // if the luminance has changed
     Serial.print("Luminance is now: "); // print it
     Serial.println(lum);
     Serial.print(". ");
     lightCharacteristic.setValue(lum);  // and update the luminance measurement characteristic
     oldLum = lum;           // save the level for next comparison
   }
}


void updateDHT() {
   /* Read the current voltage level on the A0 analog input pin.
      This is used here to simulate the heart rate's measurement.
   */
  int tempValue = random(1023);
  int humValue = random(1023);
  
  int temp = map(tempValue, 0, 1023, 0, 100);
  int hum = map(humValue, 0, 1023, 0, 100);
  
  if (temp != oldTemp) {      // if the temperature has changed
    Serial.print("Temperature: "); // print it to serial
    Serial.print(temp);
    Serial.print(". "); 
    tempCharacteristic.setValue(temp);  // and update the temperature measurement characteristic
    oldTemp = temp;           // save the value for next comparison
  }
  
  if (hum != oldHum) {      // if the humidity has changed
    Serial.print("Humidity: "); // print it to serial
    Serial.print(hum);
    Serial.print(". ");
    humCharacteristic.setValue(hum);  // and update the humidity measurement characteristic
    oldHum = hum;           // save the value for next comparison 
  }
}


void updatepH() {
   /* Read the current voltage level on the A0 analog input pin.
      This is used here to simulate the heart rate's measurement.
   */
  int pHValue = random(1023);
   int pH = map(pHValue, 0, 1023, 0, 100);
   if (pH != oldpH) {      // if the pH has changed
     Serial.print("pH is now: "); // print it
     Serial.print(pH);
     Serial.print(". ");
     phCharacteristic.setValue(pH);  // and update the pH measurement characteristic
     oldpH = pH;           // save the level for next comparison
   }
}

