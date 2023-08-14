/*****
Author: Rud Lucien
Date:   08-11-2023
Purose: Create an Arduino sketch for taking temperature and humidity measurements with a DHT22/AM2302 sensor 

Board:  Type-C USB ESP-WROOM-32 ESP32 ESP-32S Development Board 2.4GHz
        Dual-Mode WiFi + Bluetooth Dual Cores Microcontroller Processor
        Integrated with Antenna RF AMP Filter AP STA for Arduino IDE

Sensors: DHT22/AM2302

Detials: This sketch is based on the DHTtester sketch from the DHT sensor library.
         The sketch outputs temperature and humidity measurements from the DHT22/AM2302 
         sensor to two serial monitors as a table, with columns for time, humidity, 
         temperature in Celsius and Fahrenheit, and heat index in Celsius and Fahrenheit. 
         The `Serial` monitor is the default serial port of the ESP32 board, while the 
         `MySerial` monitor is a user-defined serial port that is connected to the RX and TX 
         pins of the ESP32 board.

*****/

#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302)

#define RX_PIN 16 // Change to your desired RX pin
#define TX_PIN 17 // Change to your desired TX pin

HardwareSerial MySerial(1); // UART1

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  MySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Start the hardware serial port
  dht.begin();

  // Print a header for the data to the serial monitor
  Serial.print(F("Time(sec)\t"));
  Serial.print(F("Humidity(%)\t"));
  Serial.print(F("Temperature(C)\t"));
  Serial.print(F("Temperature(F)\t"));
  Serial.print(F("Heat Index(C)\t"));
  Serial.print(F("Heat Index(F)\n"));

  // Print a header for the data to the second serial monitor
  MySerial.print(F("Time(sec)\t"));
  MySerial.print(F("Humidity(%)\t"));
  MySerial.print(F("Temperature(C)\t"));
  MySerial.print(F("Temperature(F)\t"));
  MySerial.print(F("Heat Index(C)\t"));
  MySerial.print(F("Heat Index(F)\n"));
}

void loop()
{
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
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // This code outputs the data to the serial monitor as a string (I don't really love this method)
  /*
  String data = String(F("Humidity: ")) + String(h) + String(F("%  Temperature: "));
  data += String(t) + String(F("°C ")) + String(f) + String(F("°F  Heat index: "));
  data += String(hic) + String(F("°C ")) + String(hif) + String(F("°F"));



  Serial.println(data);
  MySerial.println(data);
  */

  // This code outputs the data to the serial monitor as a table (I like this method better)
  Serial.print(millis() / 1000.0);
  Serial.print(F("\t"));
  Serial.print(h);
  Serial.print(F("\t"));
  Serial.print(t);
  Serial.print(F("\t"));
  Serial.print(f);
  Serial.print(F("\t"));
  Serial.print(hic);
  Serial.print(F("\t"));
  Serial.println(hif);

  // This code outputs the data to the second serial monitor as a table (I like this method better)
  MySerial.print(millis() / 1000.0);
  MySerial.print(F("\t"));
  MySerial.print(h);
  MySerial.print(F("\t"));
  MySerial.print(t);
  MySerial.print(F("\t"));
  MySerial.print(f);
  MySerial.print(F("\t"));
  MySerial.print(hic);
  MySerial.print(F("\t"));
  MySerial.println(hif);
}
