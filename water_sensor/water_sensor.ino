/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AnalogInput

 */

int sensorPin = 1;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
  Serial.println("Analog input read");
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // print value
  Serial.print("Water sensor: ");
  Serial.print(sensorValue);
  Serial.println("V");
  delay(2000);
}

/* AnalogInput example, 2016-03-03
 *  2 second polling
 * Water sensor: 2V
Water sensor: 1V
Water sensor: 2V
Water sensor: 2V
Water sensor: 2V
Water sensor: 2V
  Held the sensor with finger
Water sensor: 95V
Water sensor: 122V
Water sensor: 134V
Water sensor: 145V
Water sensor: 152V
  Let go
Water sensor: 26V
Water sensor: 9V
Water sensor: 2V
Water sensor: 2V
Water sensor: 2V
Water sensor: 1V
Water sensor: 2V
Water sensor: 1V
Water sensor: 1V
Water sensor: 2V
Water sensor: 1V
Water sensor: 2V
Water sensor: 1V
Water sensor: 1V
Water sensor: 1V
Water sensor: 2V
Water sensor: 1V
Water sensor: 1V
Water sensor: 2V
Water sensor: 2V
Water sensor: 1V
  Dipped in glass of water ~0.5
Water sensor: 537V
  ~1.0
Water sensor: 986V
  ~1.5
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
  ~4.0
Water sensor: 1023V
  Slowly moving it up and down to get a rough idea of sensitivity
Water sensor: 994V
Water sensor: 968V
Water sensor: 912V
Water sensor: 853V
Water sensor: 787V
Water sensor: 648V
Water sensor: 662V
Water sensor: 659V
Water sensor: 655V
Water sensor: 656V
Water sensor: 649V
Water sensor: 958V
Water sensor: 977V
Water sensor: 1023V
Water sensor: 1010V
Water sensor: 937V
Water sensor: 926V
Water sensor: 932V
Water sensor: 945V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 983V
Water sensor: 943V
Water sensor: 866V
Water sensor: 789V
  Removed from water
Water sensor: 488V
Water sensor: 476V
Water sensor: 452V
Water sensor: 431V
Water sensor: 416V
Water sensor: 393V
Water sensor: 373V
Water sensor: 360V
Water sensor: 352V
Water sensor: 341V
Water sensor: 326V
Water sensor: 311V
Water sensor: 294V
Water sensor: 277V
Water sensor: 262V
Water sensor: 266V
  Wiped off with fingers
Water sensor: 18V
Water sensor: 15V
Water sensor: 11V
Water sensor: 8V
  Drops of water sprinkled and wiped
Water sensor: 617V
Water sensor: 4V
Water sensor: 2V
Water sensor: 53V
Water sensor: 1007V
Water sensor: 1022V
Water sensor: 1023V
Water sensor: 1021V
Water sensor: 626V
Water sensor: 516V
Water sensor: 572V
Water sensor: 0V
Water sensor: 0V
Water sensor: 921V
Water sensor: 1V
Water sensor: 0V
Water sensor: 0V
Water sensor: 43V
Water sensor: 60V
Water sensor: 74V
Water sensor: 101V
Water sensor: 101V
Water sensor: 0V
Water sensor: 0V
  Dipping in water at various levels again
Water sensor: 669V
Water sensor: 740V
Water sensor: 782V
Water sensor: 809V
Water sensor: 840V
Water sensor: 862V
Water sensor: 870V
Water sensor: 879V
Water sensor: 889V
Water sensor: 893V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 1023V
Water sensor: 870V
Water sensor: 788V
Water sensor: 859V
Water sensor: 516V
Water sensor: 535V
Water sensor: 548V
Water sensor: 555V
  Removed and wiped off
Water sensor: 40V
Water sensor: 26V
Water sensor: 17V
Water sensor: 12V
Water sensor: 16V
Water sensor: 9V
Water sensor: 7V
Water sensor: 5V

 */

/*
void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue);
}
*/
