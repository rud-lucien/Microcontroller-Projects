#include <M5Core2.h>


void setup()
{
  // put your setup code here, to run once:
  pinMode(23, OUTPUT);

  M5.begin();
  M5.lcd.setTextSize(1);
  M5.lcd.println("M5Stack relay test");
}

void loop()
{
   //put your main code here, to run repeatedly:
  if (M5.BtnA.isPressed())
  {
    digitalWrite(23, HIGH);
  }
  else if (M5.BtnA.isReleased())
  {
    digitalWrite(23, HIGH);
  }

  if(M5.BtnA.wasPressed())
  {
     M5.lcd.println("Button A was pressed");
  }
  M5.update();
}