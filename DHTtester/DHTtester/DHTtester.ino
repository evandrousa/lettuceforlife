// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <DHT.h>

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}



/* Example output 2016-03-03
 *  Increase in temperature is seen when I breath on the sensor
 *  
Humidity: 41.00 %  Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 25.00 *C 77.00 *F  Heat index: 24.63 *C 76.33 *F
Humidity: 41.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.73 *C 78.31 *F
Humidity: 41.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.73 *C 78.31 *F
Humidity: 41.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.73 *C 78.31 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 40.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 25.70 *C 78.26 *F
Humidity: 41.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 26.91 *C 80.44 *F
Humidity: 42.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 26.96 *C 80.54 *F
Humidity: 44.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.07 *C 80.73 *F
Humidity: 45.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 28.03 *C 82.45 *F
Humidity: 46.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 28.11 *C 82.60 *F
Humidity: 51.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 31.21 *C 88.18 *F
Humidity: 54.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 31.71 *C 89.08 *F
Humidity: 57.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 32.25 *C 90.05 *F
Humidity: 57.00 % Temperature: 31.00 *C 87.80 *F  Heat index: 34.12 *C 93.42 *F
Humidity: 58.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 32.44 *C 90.39 *F
Humidity: 58.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 32.44 *C 90.39 *F
Humidity: 59.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 32.63 *C 90.74 *F
Humidity: 59.00 % Temperature: 30.00 *C 86.00 *F  Heat index: 32.63 *C 90.74 *F
Humidity: 59.00 % Temperature: 29.00 *C 84.20 *F  Heat index: 30.88 *C 87.59 *F
Humidity: 59.00 % Temperature: 29.00 *C 84.20 *F  Heat index: 30.88 *C 87.59 *F
Humidity: 59.00 % Temperature: 29.00 *C 84.20 *F  Heat index: 30.88 *C 87.59 *F
Humidity: 60.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 29.45 *C 85.01 *F
Humidity: 59.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 29.34 *C 84.81 *F
Humidity: 59.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 29.34 *C 84.81 *F
Humidity: 59.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 29.34 *C 84.81 *F
Humidity: 59.00 % Temperature: 28.00 *C 82.40 *F  Heat index: 29.34 *C 84.81 *F
Humidity: 59.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 28.01 *C 82.42 *F
Humidity: 59.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 28.01 *C 82.42 *F
Humidity: 58.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.94 *C 82.29 *F
Humidity: 58.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.94 *C 82.29 *F
Humidity: 57.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.87 *C 82.17 *F
Humidity: 57.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.87 *C 82.17 *F
Humidity: 56.00 % Temperature: 27.00 *C 80.60 *F  Heat index: 27.80 *C 82.04 *F
Humidity: 57.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 26.82 *C 80.27 *F
Humidity: 56.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 26.78 *C 80.21 *F
Humidity: 56.00 % Temperature: 26.00 *C 78.80 *F  Heat index: 26.78 *C 80.21 *F

 */
