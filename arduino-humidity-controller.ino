//  Nick Kelley Humidifier Control
//  Uses DHT22 humidity/temperature sensor to control relay
//  based on current humidity.
//  Requires Adafruit's DHT library:
//  https://learn.adafruit.com/dht/downloads
//  Thanks to ladyada and adafruit.com!

#include "DHT.h"

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
// highest and lowest recorded values
// (initialized to impossible values so it updates on the first loop)
float high = 0, low = 100;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT); 
}

void loop() {
  // Wait 10 seconds between measurements.
  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

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
  if (h > high) {
    high = h;
  }
  if (h < low) {
    low = h;
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("\t\tHigh: ");
  Serial.print(high);
  Serial.print("\tLow: ");
  Serial.print(low);
  Serial.print("\n");
}
