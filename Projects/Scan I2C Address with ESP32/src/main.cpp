#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 21  // Change to your desired SDA pin
#define SCL_PIN 22  // Change to your desired SCL pin

void setup() {
  Serial.begin(57600);
  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C on your chosen pins
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 0; address < 128; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000);           // wait 5 seconds for next scan  
}