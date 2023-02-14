#include <M5Stack.h>

void setup()
{
  // put your setup code here, to run once:

  // Initialize the M5Stack object
  M5.begin();
  M5.lcd.setTextSize(2); // Set font size
  M5.Lcd.setCursor(60, 100);
  M5.Lcd.print("Hello World");
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  
}

void loop()
{
  
}