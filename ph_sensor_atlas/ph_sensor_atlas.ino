
//This code was written to be easy to understand.
//Code efficiency was not considered.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the pH circuit.
//This code was written in the Arduino 1.6.5 IDE
//An Arduino UNO was used to test this code.


#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it.
#define rx 0                                          //define what pin rx is going to be.
#define tx 1                                          //define what pin tx is going to be.

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work.


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                 //have we received all the data from the PC
boolean sensor_stringcomplete = false;                //have we received all the data from the Atlas Scientific product
float ph;                                             //used to hold a floating point number that is the pH. 



void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  char inchar = (char)Serial.read();                  //get the char we just received
  inputstring += inchar;                              //add it to the inputString
  if (inchar == '\r') {                               
    input_stringcomplete = true;                      //if the incoming character is a <CR>, set the flag
  }
}

void loop() {                                         //here we go...

  if (input_stringcomplete) {                         //if a string from the PC has been received in its entirety                     
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    inputstring = "";                                 //clear the string
    input_stringcomplete = false;                     //reset the flag used to tell if we have received a completed string from the PC      
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character.
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;
    if (inchar == '\r') {
      sensor_stringcomplete = true;                   //if the incoming character is a <CR>, set the flag
    }
  }


  if (sensor_stringcomplete) {                        //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(sensorstring);                     //send that string to the PC's serial monitor
    ph = sensorstring.toFloat();                      //convert the string to a floating point number so it can be evaluated by the Arduino

    if (ph >= 7.0) {                                  //if the pH is greater than or equal to 7.0
      Serial.println("high");                         //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
    }

    if (ph <= 6.999) {                                //if the pH is less than or equal to 6.999
      Serial.println("low");                          //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
    }

    sensorstring = "";                                //clear the string:
    sensor_stringcomplete = false;                    //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}

