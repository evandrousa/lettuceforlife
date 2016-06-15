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
    
 */


// RELAY CONTROL FOR PUMP AND LIGHT
#define RELAY1_PIN 3 // Digital pin for Light
#define RELAY2_PIN 4 // Digital pin for Pump 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 2 and 3 as an output.
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {


  // turn the signal to GPIOs pin of Relay 1 to high and Relay 2 to low(HIGH the voltage level)
  digitalWrite(RELAY1_PIN, HIGH);  
  digitalWrite(RELAY2_PIN, LOW);   
  
  delay(2*1000); // wait for 2 second

  // turn the signal to GPIOs pin of Relay 2 to high and Relay 1 to low(HIGH the voltage level)
  digitalWrite(RELAY1_PIN, LOW);  
  digitalWrite(RELAY2_PIN, HIGH);  
    
  delay(2*1000); // wait for 2 second

  //Turn both relay to OFF
  digitalWrite(RELAY1_PIN, LOW); 
  digitalWrite(RELAY2_PIN, LOW);  
  
  delay(5*1000); // wait for 5 second
}
