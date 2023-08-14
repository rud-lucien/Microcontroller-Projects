#include <Arduino.h>
#include <HardwareSerial.h>

#define RX_PIN 3  // Change to your desired RX pin
#define TX_PIN 1  // Change to your desired TX pin

HardwareSerial MySerial(1);  // UART1

void setup() {
  Serial.begin(115200);  // Start the built-in serial port
  MySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);  // Start the hardware serial port
}

void loop() {
  if (MySerial.available()) {  // If data is available to read,
    char c = MySerial.read();  // read it and store it in 'c'
    Serial.print(c);  // Print the data received to the built-in serial port
  }
}