# Arduino Humidistat
Using a DHT22 sensor, an Arduino, and a relay, turns a humidifier on and off based on current humidity.

I programmed this for the Arduino Uno, though as far as I know there's no reason it couldn't easily run on other Arduino boards. If you have any questions, please email me at nick@nickkelleystuff.com.

## Required Parts

* Arduino Uno
* DHTxx sensor - I used the DHT22, available from Adafruit here: https://www.adafruit.com/product/393
	* *NOTE: There are other DHT sensors that can work with the same code, such as the DHT11. You'll need to un-comment the relevant lines of code from lines 13-16 in the source.*
* Relay - The main requirement is that the relay should be on (powered) by default. I used this relay from Adafruit, since it has options for both: https://www.adafruit.com/product/2935
* Humidifier - any humidifier that plugs into a wall will work.

## Setup

1. Wire the hardware
	* Connect the sensor to the Arduino ([Adafruit instructions here](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor)). The data pin should connect to pin #2 on the Arduino.
	* Pin 12 on the Arduino should go to the positive header on the relay, and the negative header from the relay goes to ground.
	* Humidifier gets plugged in to the "Normally ON" plug on the relay.
2. Download the libraries for the DHT sensor: https://learn.adafruit.com/dht/downloads
	* You'll need both the Adafruit Sensor library and the library for the DHT sensors.
3. Check the constants in the code.
	* If you're using a sensor other than the DHT22, comment out line 15 and un-comment the correct sensor.
	* Lines 22 and 23 define the humidity range. I have it set up to keep things around 50% relative humidity (a comfy climate for a cello and for people). Your needs may vary.
	* If you need to use other pins for data input/relay output, adjust lines 10 and 11 as needed.
4. Upload the sketch to the Arduino via USB.
	* If you'd like, you can also use the serial monitor to get log messages back from the Arduino. Since I was paranoid I did something wrong, I ran the setup this way for a bit to make sure that I was getting readings that looked right.
5. The humidifier will turn on for 10 seconds before the first reading from the sensor. At that point, if it is above the MAX value, it will turn off.
	* If it goes above the MAX value, it will turn off. If it is below the MIN value, it will turn on. If humidity is in between MAX and MIN, it will remain in its current state until crossing either threshold.
	* By design, the default state of the humidifier is on. If there is a problem reading from the sensor, the code opts to switch it back on; similarly, if the Arduino fails, the relay's default state is on.
