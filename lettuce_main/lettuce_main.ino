/*********************************************************************
  This code is based on the ssd1306_128x64_i2c example.

*********************************************************************/
//SENSORS FLAGS
bool DHT_sensor_flag    = true;
bool light_sensor_flag  = true;
bool water_sensor1_flag = true;
bool water_sensor2_flag = false;
bool ph_sensor_flag     = true;
bool bluetooth_flag     = true;

// DISPLAY LIBRARIES
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>   
#include <Adafruit_SSD1306.h>

// DIGITAL TEMP/HUMID SENSOR PIN and TYPE
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const bool inFahrenheit_f = true;
uint16_t oldHum, oldTemp;

// LIGHT SENSOR
#define LIGHTSENSOR_PIN 0 // Analog pin
uint16_t LIGHT_TIME_OFF = 8; // Time in Hours for light to be on
uint16_t pre_lum = 0;
uint16_t pre_pre_lum = 0;

// WATER LEVEL SENSORS
#define WATERLEVEL_1_PIN 1  // Analog pin
#define WATERLEVEL_2_PIN 2  // Analog pin
uint16_t pre_water_sensor1  = 0;
uint16_t pre_pre_water_sensor1  = 0;
uint16_t pre_water_sensor2  = 0;
uint16_t pre_pre_water_sensor2 = 0;

// RELAY CONTROL FOR PUMP AND LIGHT
#define RELAY1_PIN 3 // Digital pin for Light
#define RELAY2_PIN 4 // Digital pin for Pump 
#define PUMP_TIME_ON 1 // Time in minutes for pump to be on
uint16_t PUMP_TIME_OFF = 5; //time in minutes for pump off

int pre_light_trigger_time[2];
int pre_pump_trigger_time[2];

// BLUETOOTH LIBRARY AND VARIABLES
#include <CurieBLE.h>
BLEPeripheral blePeripheral; // create peripheral instance
BLEService lflService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
// create switch characteristic and allow remote device to read and write
BLECharCharacteristic r1Characteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic r2Characteristic("19B10012-E8F2-537E-4F6C-D104768A1214",  BLERead | BLEWrite);
BLECharCharacteristic lightCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); 
BLECharCharacteristic tempCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLECharCharacteristic humCharacteristic("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLECharCharacteristic phCharacteristic("19B10016-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);



// RELAY TIMER LIBRARY AND VARIABLES
#include <CurieTime.h>
int ch1TimerTrigger[2];
int ch2TimerTrigger[2];
int ch1TimerState = 1;
int ch2TimerState = 1;

//PH SENSOR LIBRARY AND VARIABLES
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it.
#define rx 0                                          //define what pin rx is going to be.
#define tx 1                                          //define what pin tx is going to be.
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work.
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_stringcomplete = false;                 //have we received all the data from the PC
boolean sensor_stringcomplete = false;                //have we received all the data from the Atlas Scientific product
float pH;                                             //used to hold a floating point number that is the pH. 
float oldpH;
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


// Display is 128x64
//   16 lines of yellow
//   black gap about 2 lines tall
//   48 lines of blue
bool showing_logo;
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#define LETTUCE_LOGO_WIDTH  128
#define LETTUCE_LOGO_HEIGHT  64
static const unsigned char PROGMEM lettuce_logo_bmp[] =
{

  // Lettuce 1
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 224, 7, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 252, 127, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 7, 192, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 1, 240, 56, 0, 120, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 56, 0, 240, 24, 0, 120, 0, 0, 120, 120, 0, 0, 0, 0, 0, 0, 56, 0, 120, 24, 0, 120, 0, 0, 120, 120, 0, 0, 0, 0, 0, 0, 254, 0, 60, 126, 0, 120, 1, 248, 254, 254, 121, 225, 248, 31, 128, 7, 255, 192, 31, 255, 192, 120, 3, 252, 254, 254, 121, 227, 252, 63, 192, 15, 255, 240, 31, 255, 224, 120, 3, 156, 254, 254, 121, 227, 158, 57, 192, 15, 199, 252, 127, 199, 240, 120, 7, 158, 120, 120, 121, 231, 152, 121, 224, 12, 0, 62, 252, 0, 112, 120, 7, 254, 120, 120, 121, 231, 128, 127, 224, 28, 0, 31, 240, 0, 112, 120, 7, 254, 120, 120, 121, 231, 128, 127, 224, 28, 0, 7, 192, 0, 112, 120, 7, 128, 120, 120, 121, 231, 152, 120, 0, 28, 0, 3, 192, 0, 48, 127, 195, 158, 120, 120, 127, 231, 158, 57, 224, 28, 0, 1, 224, 0, 48, 127, 195, 252, 126, 126, 127, 227, 252, 63, 192, 28, 0, 0, 240, 0, 48, 127, 193, 248, 62, 62, 61, 225, 248, 31, 128, 28, 0, 0, 120, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 56, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 3, 0, 60, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 3, 0, 28, 0, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 7, 0, 14, 0, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 3, 128, 14, 0, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 3, 128, 14, 0, 112, 127, 192, 0, 0, 1, 224, 30, 62, 0, 0, 14, 3, 128, 6, 0, 224, 127, 192, 0, 0, 1, 224, 30, 126, 0, 0, 7, 3, 128, 7, 0, 224, 127, 192, 0, 0, 1, 224, 0, 120, 0, 0, 7, 1, 128, 7, 1, 224, 120, 0, 240, 123, 129, 224, 30, 254, 63, 0, 7, 129, 192, 7, 1, 192, 120, 3, 252, 127, 129, 224, 30, 254, 127, 128, 3, 129, 192, 7, 3, 192, 127, 131, 156, 127, 1, 224, 30, 254, 115, 128, 3, 192, 224, 7, 7, 128, 127, 135, 158, 124, 1, 224, 30, 120, 243, 192, 1, 224, 240, 7, 7, 0, 127, 135, 158, 120, 1, 224, 30, 120, 255, 192, 0, 240, 120, 7, 15, 0, 120, 7, 158, 120, 1, 224, 30, 120, 255, 192, 0, 240, 60, 7, 30, 0, 120, 7, 158, 120, 1, 224, 30, 120, 240, 0, 0, 124, 31, 7, 124, 0, 120, 3, 156, 120, 1, 255, 30, 120, 115, 192, 0, 62, 15, 199, 248, 0, 120, 3, 252, 120, 1, 255, 30, 120, 127, 128, 0, 15, 135, 255, 240, 0, 120, 0, 240, 120, 1, 255, 30, 120, 63, 0, 0, 7, 243, 255, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

  // Bentley
  //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 255, 255, 192, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 240, 31, 255, 255, 255, 255, 255, 255, 252, 127, 255, 255, 255, 255, 128, 127, 0, 1, 254, 1, 255, 255, 255, 255, 254, 112, 0, 0, 127, 128, 0, 14, 0, 0, 96, 0, 1, 252, 0, 0, 14, 62, 0, 0, 1, 255, 254, 6, 0, 0, 96, 255, 255, 128, 0, 0, 124, 31, 255, 255, 255, 206, 3, 134, 0, 0, 97, 192, 243, 255, 255, 255, 240, 15, 255, 255, 192, 199, 0, 204, 127, 252, 51, 0, 231, 3, 255, 255, 240, 14, 0, 0, 31, 255, 0, 89, 255, 255, 26, 1, 255, 248, 0, 0, 112, 7, 255, 255, 254, 115, 128, 247, 255, 255, 206, 3, 142, 127, 255, 255, 224, 1, 255, 252, 3, 249, 192, 255, 255, 255, 247, 7, 159, 128, 127, 255, 128, 0, 224, 0, 255, 187, 241, 255, 192, 15, 255, 143, 217, 254, 0, 7, 0, 0, 112, 255, 240, 60, 251, 127, 3, 135, 254, 254, 60, 31, 254, 14, 0, 0, 63, 254, 3, 255, 191, 255, 135, 195, 255, 253, 255, 192, 127, 252, 0, 0, 15, 0, 127, 15, 79, 255, 135, 195, 255, 246, 241, 252, 0, 240, 0, 0, 15, 143, 248, 123, 211, 255, 135, 195, 255, 219, 252, 31, 227, 224, 0, 0, 3, 255, 129, 227, 245, 255, 135, 195, 255, 175, 135, 129, 255, 192, 0, 0, 0, 112, 15, 135, 127, 255, 131, 135, 255, 254, 225, 240, 30, 0, 0, 0, 0, 127, 252, 60, 255, 255, 128, 15, 255, 255, 56, 63, 252, 0, 0, 0, 0, 31, 224, 243, 159, 255, 129, 7, 255, 251, 143, 7, 248, 0, 0, 0, 0, 1, 255, 135, 55, 255, 131, 195, 255, 204, 227, 255, 128, 0, 0, 0, 0, 0, 255, 28, 103, 255, 135, 225, 255, 102, 122, 255, 0, 0, 0, 0, 0, 0, 23, 251, 205, 255, 135, 225, 255, 51, 191, 224, 0, 0, 0, 0, 0, 0, 1, 223, 253, 255, 135, 225, 255, 191, 243, 128, 0, 0, 0, 0, 0, 0, 0, 4, 255, 255, 135, 193, 255, 254, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 127, 3, 131, 252, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 127, 192, 7, 252, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 63, 252, 127, 240, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 15, 255, 255, 224, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 7, 255, 255, 192, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 1, 255, 255, 1, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 129, 255, 255, 3, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 195, 122, 189, 135, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 242, 90, 180, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 214, 150, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 150, 210, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 182, 219, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 182, 219, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif



//********************************************************************
//******************** START INITIAL SETUP ***************************
//********************************************************************

void setup()   {

  // set the current time to 00:00:00, January 1st, 2016
  setTime(0, 0, 30, 1, 1, 2016);

  Serial.begin(9600);
  
  // Bluetooth peripheral advertises
  if (bluetooth_flag == true) {
    // set the local name peripheral advertises
    blePeripheral.setLocalName("Lettuce");
    // set the UUID for the service this peripheral advertises:
    blePeripheral.setAdvertisedServiceUuid(lflService.uuid());
    
    // add service and characteristics
    blePeripheral.addAttribute(lflService);
    blePeripheral.addAttribute(r1Characteristic);
    blePeripheral.addAttribute(r2Characteristic);
    blePeripheral.addAttribute(lightCharacteristic);
    blePeripheral.addAttribute(tempCharacteristic);
    blePeripheral.addAttribute(humCharacteristic);
    blePeripheral.addAttribute(phCharacteristic);
    // Set initial Values
    r1Characteristic.setValue(LIGHT_TIME_OFF);
    r2Characteristic.setValue(PUMP_TIME_OFF);
    // assign event handlers for connected, disconnected to peripheral
    blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    // advertise the service
    blePeripheral.begin();
    Serial.println(("Bluetooth device active, waiting for connections..."));
  } 

  // Initialize.
  pinMode(RELAY1_PIN, OUTPUT); // Currently connected for Light
  pinMode(RELAY2_PIN, OUTPUT); // Currently connected for Water Pump
  pinMode(LIGHTSENSOR_PIN, INPUT);
  dht.begin();
  pinMode(WATERLEVEL_1_PIN, INPUT);
  pinMode(WATERLEVEL_2_PIN, INPUT);

  //Start the device with both relays ON
  digitalWrite(RELAY1_PIN, 1);
  digitalWrite(RELAY2_PIN, 1);

  
  // Check if any reads failed to disable sensors  
  if isnan(analogRead(LIGHTSENSOR_PIN)) {
    //light_sensor_flag = false;
  }   
  if (isnan(dht.readHumidity()) || isnan(dht.readTemperature(true))) {
    Serial.print("Failed to read from DHT sensor!");
  //    DHT_sensor_flag = false;
  } 
  if isnan(analogRead(WATERLEVEL_1_PIN)) {
    water_sensor1_flag = false;
  }   
  if isnan(analogRead(WATERLEVEL_2_PIN)) {
    water_sensor2_flag = false;
  } 
  
  //pH Sensor Init
  if (ph_sensor_flag == true) {
    myserial.begin(9600);                               //set baud rate for software serial port_3 to 9600
    inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
    sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  }
     
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  showing_logo = false;

  // Display our growing lettuce animation
  do_logo_animation(lettuce_logo_bmp, LETTUCE_LOGO_WIDTH, LETTUCE_LOGO_HEIGHT);
  showing_logo = true;

  // Display our lettuce logo for 10 seconds
  //display.drawBitmap(XPOS, YPOS, bitmap, w, h, WHITE);
  //display.drawBitmap(0, 0,  lettuce_logo_bmp, 128, 64, 1);
  //display.display();
  //delay(5000);

  // Clear the buffer.
  //display.clearDisplay();
}

typedef struct sys_readings_s {
  float temperature;
  float humidity;
  uint16_t water_level;
  int pH;
  uint16_t light_level;
} sys_readings_t;


//********************************************************************
//********************** END INITIAL SETUP ***************************
//********************************************************************

//********************************************************************
//************************ START MAIN LOOP ***************************
//********************************************************************

void loop() {

  //Print current time to Serial Output
  Serial.print("Time: ");
  print2digits(hour());
  Serial.print(":");
  print2digits(minute());
  Serial.print(":");
  print2digits(second());
  Serial.print(" | R1OFF: ");
  Serial.print(LIGHT_TIME_OFF);
  Serial.print(" | R2OFF: ");
  Serial.print(PUMP_TIME_OFF);
  Serial.print(" | ");
    

  //Show logo to LCD
  display.clearDisplay();
  display.drawBitmap(0, 0,  lettuce_logo_bmp, 128, 64, 1);
  display.display();
  showing_logo = true;

  
  // display status to LCD
  display.setTextSize(1);
  display.setCursor(0, 0);
  //display.setTextColor(WHITE);
  Serial.print("Reporting ");
  display.setTextColor(WHITE);
  //display.println("STATUS: OK");
  Serial.print("STATUS: OK | ");
   display.display();

//========== LIGHT SENSOR - START ============
  if (light_sensor_flag == true) {
    updateLuminance();
  } else {
    Serial.print("No Light Sensor | ");
    display.print("L: OFF");
  }
//========== LIGHT SENSOR - END  =============
//========== DHT SENSOR - START ==============
  if (DHT_sensor_flag == true) {
     updateDHT();
  } else {
    Serial.print("No DHT Sensor | ");
    display.println("T & H:OFF ");
  }
//========== DHT SENSOR - END  ===============
//======= WATER LEVEL 1 SENSOR - START =======
 // bool getting_water1;
 // bool getting_water2;
  if (water_sensor1_flag == true) {
    
    //Gather water sensor reading from sensor and converts it to percentage 
    uint16_t cur_water_sensor1 = (analogRead(WATERLEVEL_1_PIN)/1023.0) * 100 ;

    //Calculates the running average of the water sensor value using past 2 readings
    uint16_t sum_water_sensor1 = (cur_water_sensor1 + pre_water_sensor1 + pre_pre_water_sensor1)/3;

    //Insert new reading, drops last reading
    pre_water_sensor1 = cur_water_sensor1;
    pre_pre_water_sensor1 = pre_water_sensor1;

   //for demo purposes, I am using water_sensor, in reality should use sum_watee_sensor
    if (cur_water_sensor1 > 25 && digitalRead(4)) {
      //int ch2_time_in_state =  minute() - ch2TimerTrigger[1];
      //if (ch2_time_in_state <0) {
      //   ch2_time_in_state+24;} 
      //const char* msg1 = "Need Water " + ch1_time_in_state + "m ago"
   //   const char* msg5 = "Sensor 1 Not Wet";
   //   flash_warning_banner(msg5,5);

   //   getting_water1 = false;
    }     
    
    //Display Water Sensor 1 information to LCD    
    display.print("W1:");
    display.print(cur_water_sensor1);
    display.print("% ");
    
    //Display Water Sensor 1 information to Serial Terminal
    Serial.print("Water Sensor 1: ");
    Serial.print(sum_water_sensor1, DEC);
    Serial.print("% | ");
    
  } else {
    Serial.print("No WL1 Sensor | ");
    display.print("W1:OFF ");
  }
  
//======= WATER LEVEL 1 SENSOR - START =======
//======= WATER LEVEL 2 SENSOR - START =======
  if (water_sensor2_flag == true) {
    
    //Gather water sensor reading from sensor and converts it to percentage 
    uint16_t cur_water_sensor2 = (analogRead(WATERLEVEL_2_PIN)/1023.0) * 100 ;

    //Calculates the running average of the water sensor value using past 2 readings
    uint16_t sum_water_sensor2 = (cur_water_sensor2 + pre_water_sensor2 + pre_pre_water_sensor2)/3;

    //Insert new reading, drops last reading
    pre_water_sensor2 = cur_water_sensor2;
    pre_pre_water_sensor2 = pre_water_sensor2;

    //for demo purposes, I am using water_sensor, in reality should use sum_watee_sensor
    if (cur_water_sensor2 > 25 && digitalRead(4)) {
      //int ch2_time_in_state =  minute() - ch2TimerTrigger[1];
      //if (ch2_time_in_state <0) {
      //   ch2_time_in_state+24;} 
      //const char* msg1 = "Need Water " + ch1_time_in_state + "m ago"
      const char* msg6 = "Sensor 1 Not Wet";
      flash_warning_banner(msg6,5);
    }

    //Display Water Sensor 2 information to LCD    
    display.print("W2:");
    display.print(cur_water_sensor2);
    display.print("% ");
    
    //Display Water Sensor 2 information to Serial Terminal
    Serial.print("Water Sensor 2: ");
    Serial.print(sum_water_sensor2, DEC);
    Serial.print("% | ");
    
  } else {  
    Serial.print("No WL2 Sensor | ");
    //display.print("W2:OFF ");
  }
//======== WATER LEVEL 2 SENSOR - END ========

//======== pH SENSOR - START ==================
  if (ph_sensor_flag == true) {
    updatepH();
  } else {
    Serial.print("No pH Sensor | ");    
    display.print("pH:OFF "); 
  }
//========== pH SENSOR - END  ==============

//========== RELAY - START  ================
  //Toggle relays on and off if necessary
  //bool cur_light_relay =  digitalRead(3);
  //bool cur_pump_relay =  digitalRead(4);

  // Pump Timer Relay Trigger
  TriggerRelay(PUMP_TIME_ON, PUMP_TIME_OFF, 2);
  
  // Light Timer Relay Trigger
  TriggerRelay(24-LIGHT_TIME_OFF, LIGHT_TIME_OFF, 1);
 

//========== RELAY - END  ==================

//========== BLUETOOTH - START ===============
  if (bluetooth_flag == true) { 
    if (r1Characteristic.written()) {
      // update Relay1 OFF time if the central has written to the characteristic
      int r1 = r1Characteristic.value();
      Serial.print("new R1 Off Value = ");
      Serial.println(r1);
      LIGHT_TIME_OFF = r1;
    }
    if (r2Characteristic.written()) {
      // update Relay1 OFF time if the central has written to the characteristic
      int r2 = r2Characteristic.value();
      Serial.print("new R2 Off Value = ");
      Serial.println(r2);
      PUMP_TIME_OFF = r2;
    }

    //lightChar.setValue(cur_luminance);
    blePeripheral.poll();
  }
//========== BLUETOOTH - END ===============

/*
  display.setCursor(50,56);
  display.print("R1:");   
  display.print(On_or_off(digitalRead(3))); 
  display.print("R2:"); 
  display.print(On_or_off(digitalRead(4))); 
*/    
  //flash_warning_banner();
    
  //display.fillRect(0, 0, display.width(), uint8_t(display.height()/5), 1);
  //display.setTextColor(BLACK);
  //display.println("STATUS: REQUIRE ATTENTION ***");
 // Serial.print("STATUS: NOT OK, System requires attention");
  
  //update OLED LCD display
  display.display();
  
  Serial.println("\n*******************************************************************************************************");
  //display_readings(&current_readings);
  
  delay(2500);
}

//********************************************************************
//************************** END MAIN LOOP ***************************
//********************************************************************

void flash_warning_banner(const char* warn_text, int line) {
  Serial.println("Sending warning to alert the user");
  Serial.println(warn_text);
  uint8_t cnt;

  if (showing_logo == true) { 
    display.clearDisplay();
    showing_logo= false;
  }
  
  for (cnt = 0; cnt < 8; cnt++) {
    display.fillRect(0, uint8_t(display.height() / 4), display.width(),display.height(),0);
 
    display.setCursor(10, 16);
    display.println("*** ATTENTION ***");
    switch (line) {
      case 1:
        display.print(warn_text);
        break;
      case 2:
        display.println(warn_text);
        break;
      case 3:
        display.println();
        display.print(warn_text);
        break;
      case 4:
        display.println();
        display.println(warn_text);
        break;
      case 5:
        display.println();
        display.println();
        display.print(warn_text);
        break;
      case 6:
        display.println();
        display.println();
        display.println(warn_text);
        break;
      case 7:
        display.println();
        display.println();
        display.println();
        display.print(warn_text);
        break;
        
      default: 
        display.clearDisplay();
        break;
       }
    delay(750);
  }
  
    display.setCursor(0, 0);
}


void do_logo_animation(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t ypos;

  Serial.println("Animating logo sequence");
  display.fillRect(0, 0, display.width(), display.height(), WHITE);

  ypos = display.height() - 16 /* num lines of yellow */;
  while (ypos > -1) {
    Serial.print("y: ");
    Serial.print(ypos, DEC);
    Serial.print(";");

    // draw logo
    display.drawBitmap(0, ypos, bitmap, w, h, BLACK);
    display.display();
    delay(200);

    // then erase it + move it
    display.drawBitmap(0, ypos, bitmap, w, h, WHITE);
    // move it
    ypos -= 2;
  }
  Serial.println("\nLogo animation complete");
} 
     

bool TriggerRelay(int onTime, int offTime, int channel) {

  int timerCurrent;
  int stateCurrent;
  int pinCurrent;
  
  if (channel == 1) {

    //Print on Serial Output
    Serial.print("\n[RelayLight] ");

    //Get time when relay was last trigger time depending on channel
    timerCurrent = hour() - ch1TimerTrigger[0];
    stateCurrent = ch1TimerState;
    pinCurrent = RELAY1_PIN;

    //Handles 60 minutes reset scenario
    if (timerCurrent < 0) {
      timerCurrent = timerCurrent + 24;
    }
  } else if (channel == 2) {
    Serial.print("\n[RelayPump] ");
    timerCurrent = minute() - ch2TimerTrigger[1];
    stateCurrent = ch2TimerState;
    pinCurrent = RELAY2_PIN;
    if (timerCurrent < 0) {
      timerCurrent = timerCurrent + 60;
    }
  } else {
    Serial.print("\n[  Relays  ] FAIL - Invalid channel number. Valid entries are 1 or 2");
    return false;
  }
  
  //for demo
  display.setCursor(48,56);
  display.print("Pump ");  
  display.print(On_or_off(stateCurrent));  
  display.print("~");
  if (stateCurrent == 0) {
    display.print(offTime-timerCurrent);
  }
  if (stateCurrent == 1) {
    display.print(onTime-timerCurrent);
  }
  display.print("Min");
  
  //breaks out of funticion if not even 1 increment happened yet
  if (timerCurrent == 0) {
    Serial.print("SKIP - Time value has not changed since last triggered. Current State: ");
    Serial.print(stateCurrent);
    display.print("STATUS: OK");
    return false;
  }

  //Print to serial monitor
  Serial.print("Time since last triggered = ");
  Serial.print(timerCurrent);

  //if channel is currently OFF, attempt to run ON
  if (stateCurrent == 0) {
    Serial.print(" | Current State = Low");
    if (timerCurrent % (offTime) == 0) {
      Serial.print(" | Triggering Relay!");
      digitalWrite(pinCurrent, 1);
      if (channel == 1) {
        ch1TimerTrigger[0] = hour();
        ch1TimerTrigger[1] = minute();
        ch1TimerState = 1;
        return true;
      } else {
        ch2TimerTrigger[0] = minute();
        ch2TimerTrigger[1] = minute();
        ch2TimerState = 1;
        return true;
      }
    }
  }
  //if channel is currently ON, attempt to run OFF
  else if (stateCurrent == 1) {
    Serial.print(" | Current State = High");
    if (timerCurrent % (onTime) == 0) {
      Serial.print(" | Triggering Relay!");
      digitalWrite(pinCurrent, 0);
      if (channel == 1) {        
        ch1TimerTrigger[0] = minute();
        ch1TimerTrigger[1] = minute();
        ch1TimerState = 0;
        return true;
      } else {
        ch2TimerTrigger[0] = minute();
        ch2TimerTrigger[1] = minute();
        ch2TimerState = 0;
        return true;
      }
    }
  } else { return false;}
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}



//************* START PH FUNCTIONS*****************
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  char inchar = (char)Serial.read();                  //get the char we just received
  inputstring += inchar;                              //add it to the inputString
  if (inchar == '\r') {                               
    input_stringcomplete = true;                      //if the incoming character is a <CR>, set the flag
  }
}

//************** END PH FUNCTIONS******************

//********* START BLUETOOTH FUNCTIONS**************
void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}
//********* END BLUETOOTH FUNCTIONS**************

//********* START UPDATES FUNCTIONS**************
void updateLuminance() {

  //Gather luminance from sensor and converts it to percentage 
  uint16_t lum = map(analogRead(LIGHTSENSOR_PIN), 0, 1023, 0, 100);
  
  if (lum != pre_lum) {      // if the luminance has changed
   
    //Calculates the running average of the luminace value using past 2 readings
    uint16_t sum_lum = (lum + pre_lum + pre_pre_lum)/3;

    //for demo purposes, I am using cur_luminance, in reality should use sum_luminance
    /*if (cur_luminance > 75 && !digitalRead(3)) {
      int ch1_time_in_state =  hour() - ch1TimerTrigger[0];
      if (ch1_time_in_state <0) {
         ch1_time_in_state+24;} 
      //const char* msg1 = "Need Dark " + ch1_time_in_state + "h ago"
      const char* msg1 = "Plant Needs Sleep";
      flash_warning_banner(msg1,3);
    } */      
    if (lum < 30){
        //if (analogRead(3)) {
          //int ch1_time_in_state2 =  hour() - ch1TimerTrigger[0];
          //int ch1_time_in_state2
          //if (ch1_time_in_state <0) {
          //   ch2_time_in_state+24;}
        //}
      digitalWrite(3,1);
      const char* msg2 = "Plant Needs Light";
      flash_warning_banner(msg2,3);
      } else {
        digitalWrite(3,0);
      }
    
    if (bluetooth_flag){
      lightCharacteristic.setValue(sum_lum);  // and update the luminance measurement characteristic
    }
    
    //Insert new reading, drops last reading
    pre_lum = lum;
    pre_pre_lum = pre_lum;
  }
  
  //Display Light Sensor information(photocel) to LCD    
  display.print("L: ");
  display.print(lum);
  display.print("% ");
  
  //Display Light Sensor information(photocel) to Serial Terminal
  Serial.print("Light Density: ");
  Serial.print(lum, DEC);
  Serial.print("% | ");
  
}

void updateDHT() {

  //Gather humidity and temperature readings from sensor
  int hum = dht.readHumidity();
  int temp = dht.readTemperature(inFahrenheit_f);
  
  if (hum != oldHum || temp != oldTemp) {      // if the humidity or Temperature has changed

    if (bluetooth_flag) {
      humCharacteristic.setValue(hum);  // and update the humidity measurement characteristic
      tempCharacteristic.setValue(temp);  // and update the temperature measurement characteristic
    }
       
    oldHum = hum;           // save the value for next comparison 
    oldTemp = temp;           // save the value for next comparison
  }
    
    //Print out sensor information to LCD.
    display.print("H: ");
    display.print(hum, 1);
    display.print("% T: ");
    display.print(temp, 1);
    if (inFahrenheit_f)
      display.println("F");
    else
      display.println("C");

    //Print out sensor information to Serial Output
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("% Temperature ");
    if (inFahrenheit_f)
      Serial.print("(F)");
    else
      Serial.print("(C)");
    Serial.print(": ");
    Serial.print(temp);
    Serial.print(" | ");    
    
}

void updatepH() {
   /* Read the current voltage level on the A0 analog input pin.
      This is used here to simulate the heart rate's measurement.
   */
  if (input_stringcomplete) {                        //if a string from the PC has been received in its entirety                     
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
    Serial.print("pH: "); 
    Serial.println(sensorstring.substring(0,3));      //send that string to the PC's serial monitor
    pH = sensorstring.toFloat();                      //convert the string to a floating point number so it can be evaluated by the Arduino
    
    if (pH != oldpH) {      // if the pH has changed
      
      if (pH >= 8) {                                    //if the pH is greater than or equal to 7.0
        Serial.print("high |");                           //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        display.print("pH:OFF ");
      } 
      else if (pH <= 4) {                             //if the pH is less than or equal to 6.999
        Serial.print("low |");                            //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
        display.print("pH:OFF ");
      } 
      else {
        display.print("pH:");
        display.print(pH);
        sensorstring = "";                                //clear the string:
        sensor_stringcomplete = false;                    //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
      }
      if (bluetooth_flag) {
        phCharacteristic.setValue(pH);  // and update the pH measurement characteristic
      }
      oldpH = pH;           // save the level for next comparison
    
    
    } else {
        display.print("pH: ");
        display.print(oldpH);
    } 
  }
}

//********* END UPDATES FUNCTIONS**************


// Transform 0 and 1 values to ON and OFF words. Return is always 3 char long
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


