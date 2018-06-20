//  Nick Kelley Humidifier Control
//  Uses DHT22 humidity/temperature sensor to control relay
//  based on current humidity.
//  Requires Adafruit's DHT library:
//  https://learn.adafruit.com/dht/downloads
//  Thanks to ladyada and adafruit.com!

#include "DHT.h"
#include <EEPROM.h>

#define DHTPIN 2      // the sensor input pin
#define RELAYPIN 12   // the relay output pin

// Uncomment whatever sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Constants for maximum and minimum humidity. Adjust for your needs.
// Humidifier will only turn on when humidity drops below minimum,
// and will remain on until above the maximum.

#define MIN 45
#define MAX 55

// Connect pin 1 (on the left) of the sensor to +3.3V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// humidifier running state
bool humidifier = true;

//EEPROM addresses for high and low humidity & temps
#define HIGHESTHUM 1
#define LOWESTHUM 2
#define HIGHESTTEMP 3
#define LOWESTTEMP 4
// check if this EEPROM address is a specific value to make sure that
// it's (probably) set up for this sketch correctly.
#define ISSETUP 0

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Because I'm OCD and don't want the LED on.

  //check to make sure that EEPROM is setup correctly for the sketch
  if(EEPROM.read(ISSETUP) != 42) {
    // set ISSETUP to 42 to pass the test on next boot
    EEPROM.write(ISSETUP, 42);
    // set high/low values for temp and humidity to impossible values
    // they will be overwritten on the first loop
    EEPROM.write(HIGHESTHUM, 0);
    EEPROM.write(LOWESTHUM, 100);
    EEPROM.write(HIGHESTTEMP, 0);
    EEPROM.write(LOWESTTEMP, 255);
  }
}

void loop() {
  // Wait 10 seconds between measurements.
  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!\n");
    digitalWrite(RELAYPIN, LOW);
    Serial.println("Turning on humidifier just in case.\n");
    humidifier = 1;
    
    return;
  }

  // Check if humidity < MIN, if so turn on humidifier.
  // Else, if h > 50%, turn it off.
  if (!humidifier && (h < MIN)) {
    Serial.print("Humidity less than MIN, turning on humidifier \n");
    digitalWrite(RELAYPIN, LOW);
    humidifier = true;
  } else if (humidifier && (h > MAX)) {
    Serial.print("Humidity greater than MAX, turning off humidifier \n");
    digitalWrite(RELAYPIN, HIGH);
    humidifier = false;
  }

  // Check current humidity against high and low values, and update if necessary.
  if (h > EEPROM.read(HIGHESTHUM)) {
    EEPROM.write(HIGHESTHUM, h);
  }
  if (h < EEPROM.read(LOWESTHUM)) {
    EEPROM.write(LOWESTHUM, h);
  }
  
  // Check current temp against high and low values, and update if necessary.
  if (t > EEPROM.read(HIGHESTTEMP)) {
    EEPROM.write(HIGHESTTEMP, t);
  }
  if (t < EEPROM.read(LOWESTTEMP)) {
    EEPROM.write(LOWESTTEMP, t);
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("\t\tHigh: ");
  Serial.print(EEPROM.read(HIGHESTHUM));
  Serial.print("\tLow: ");
  Serial.print(EEPROM.read(LOWESTHUM));
  Serial.print("\n");
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("\t\tHigh: ");
  Serial.print(EEPROM.read(HIGHESTTEMP));
  Serial.print("\tLow: ");
  Serial.print(EEPROM.read(LOWESTTEMP));
  Serial.print("\n\n");
}
