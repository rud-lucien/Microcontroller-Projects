#include <M5Core2.h>
#include <TimeLib.h>

unsigned long previousTime = 0;
byte seconds;
byte minutes;
byte hours;
byte days;

void redText()
{
  M5.Lcd.setTextColor(RED, BLACK);
}

void yellowText()
{
  M5.Lcd.setTextColor(YELLOW, BLACK);
}

void greenText()
{
  M5.Lcd.setTextColor(GREEN, BLACK);
}

void orangeText()
{
  M5.Lcd.setTextColor(ORANGE, BLACK);
}

void blueText()
{
  M5.Lcd.setTextColor(BLUE, BLACK);
}

void purpleText()
{
  M5.Lcd.setTextColor(PURPLE, BLACK);
}

void TextColortoDefault()
{
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void textColor()
{
  if (seconds > 15 && seconds <= 30)
  {
    yellowText();
  }
  else if (seconds > 30 && seconds <= 45)
  {
    orangeText();
  }
  else if (seconds > 45 && seconds <= 60)
  {
    redText();
  }
  else
  {
    TextColortoDefault();
  }
}

void setup()
{
  M5.begin();
  Serial.begin(115200);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.println("M5 has been running for:");
}

void loop()
{
  if (millis() >= (previousTime))
  {
    previousTime = previousTime + 1000; // use 100000 for uS
    seconds = seconds + 1;
    if (seconds == 60)
    {
      seconds = 0;
      minutes = minutes + 1;
    }
    if (minutes == 60)
    {
      minutes = 0;
      hours = hours + 1;
    }
    if (hours == 24)
    {
      hours = 0;
      days = days + 1;
    }

    textColor();
    M5.Lcd.setCursor(10, 30);
    M5.Lcd.print(days, DEC);
    M5.Lcd.print(" Days|");
    M5.Lcd.print(" ");
    M5.Lcd.print(hours, DEC);
    M5.Lcd.printf(" Hours|");
    M5.Lcd.print(" ");
    M5.Lcd.print(minutes, DEC);
    M5.Lcd.print(" Minutes|");
    M5.Lcd.print(" ");
    M5.Lcd.print(seconds, DEC);
    M5.Lcd.print(" Seconds ");

    // Serial.print(days, DEC);
    // Serial.print(":");
    // Serial.print(hours, DEC);
    // Serial.print(":");
    // Serial.print(minutes, DEC);
    // Serial.print(":");
    // Serial.println(seconds, DEC);

  } // end 1 second
} // end loop
