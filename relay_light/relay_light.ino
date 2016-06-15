/*

Relay Timed Controller for Lettuce For Life Project
Author: Evandro Gomes 2/14/16

This module uses an adjustable timer to control a relay that turns water pump or lights of the system on and off.  
The Arduino only controls the contacts inside the relay, it does use or give power to the actual components being controlled 

CAUTION: Do not exceed the rated voltage and current limitations associated with the relay contacts you use in building this circuit.
CAUTION: Only use this module with a relay board rated to be able to be used by the Arduino used in this project.
CAUTION: Use a diode to bypass coil reverse voltage spike associated with all coils so you don't destroy your Arduino!

CONTROLS: 
    - adjust the time parameter for water pump functionto increase/decrease the timer delay
    - adjust the time parameter for light functionto increase/decrease the timer delay



Light Sensor for Lettuce For Life Project
Author: Evandro Gomes 2/28/16

This module uses an light diode to send a signal to a relay to turn the grow light on/off, depending on how much light is detected.  

This scrpit uses a YUROBOT(aka wRobot) light sensor with 3 pins(OUT, VCC and GND). Please connect OUT to any AnalogIn A0-A7 pin, VCC to 5V Power pin, and GND to GND Power pin.

The sensor schematic consist of a light dependent resistor connected between Vcc and Out, and the 10K resistor in parallel with the 0.1uF capacitor connected between Out and Ground.
 
    
 */

#include <CurieTime.h>


// RELAY CONTROL FOR PUMP AND LIGHT
#define RELAY1_PIN 3 // Digital pin for Relay Channel that controls CH1 
#define RELAY2_PIN 4 // Digital pin for Relay Channel that controls CH2
#define LIGHTSENSOR_PIN 0 //Analog Pin for Light 

// the setup function runs once when you press reset or power the board
void setup() { 

  //Initialize Serial to later print LightSensor value on Serial Monitor
  Serial.begin(9600); 
  
  // Initialize Relay Pins.
  pinMode(RELAY1_PIN, OUTPUT); // Currently connected for Light
  pinMode(RELAY2_PIN, OUTPUT); // Currently connected for Water Pump
}

// the loop function runs over and over again forever
void loop() {

  
    //Read the Analog Pig connected to LightSensor and save value to Luminance variable
  uint16_t cur_luminance = analogRead(LIGHTSENSOR_PIN);
  
  //Print Luminance Value to Serial Monitor
  Serial.print("Luminance Value: ");
  Serial.print(cur_luminance,DEC);
  Serial.print(" | ");

  //Logic that uses luminance value to control relay
  if (cur_luminance <=350) {  
    //Turn all Relay channels to HIGH
    digitalWrite(RELAY1_PIN, HIGH);   
    digitalWrite(RELAY2_PIN, HIGH);
   }
   else {
    //Turn all Relay channels to LOW
    digitalWrite(RELAY1_PIN, LOW);  
    digitalWrite(RELAY2_PIN, LOW);  
   }

  //Read relay pin state and print to Serial Monitor 
  Serial.print("Relay 1 Status: ");
  Serial.print(On_or_off(digitalRead(RELAY1_PIN)));
  Serial.print(" | Relay 2 Status: ");
  int relay2 = Serial.print(On_or_off(digitalRead(RELAY2_PIN)));
  
  // Sleeps for 2 second
  delay(2*1000); 
  Serial.println("\n*******************************************************************************************************");
             
}
const char* On_or_off(int pin_value) {
  if (pin_value == 0) {
    return "OFF";
  }   
  else if (pin_value == 1) {
    return "ON ";
  } 
  else {
    return "BAD";
  }
}


