#include <M5Core2.h>

// This M5Stack Core 2 script controls two relays serial data
// from the serial monitor, capturing the individual
// characters, then evaluating the contents them to see what
// ostate the relas should be in. Relay packet design is as follows:
// 	<00> : all relays off
//	<01> : relay 1 on
//	<10> : relay 2 on
//	<11> : both relays on
//  <f>  : flush Serial1 write
unsigned long previousTime = 0;
byte seconds;
byte minutes;
byte hours;
byte days;
const int RELAY_1 = 32; // the board pin, which connects to the IN pin of relay 1
const int RELAY_2 = 33; // the board pin, which connects to the IN pin of relay 2
const unsigned long timeout_ms = 20;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup()
{
  M5.begin();
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 13, 14); // Configure serial port that will be read to control relays
  pinMode(RELAY_1, OUTPUT);                  // Set pin 21 to output mode.
  pinMode(RELAY_2, OUTPUT);                  // Set pin 22 to output mode.
  digitalWrite(RELAY_1, LOW);                // Set relay 1 to off at startup
  digitalWrite(RELAY_2, LOW);                // Set relay 2 to off at startup
  M5.Lcd.print("Relay Control with Serial RS-232: v4");
  M5.Lcd.drawLine(0, 10, 320, 10, BLUE);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println("M5 has been running for:");
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.print("<M5 is ready for relay commands>");
  Serial1.write("<M5 is ready for relay commands>");
  Serial1.flush();
  Serial.print("<M5 is ready for relay commands>");
  Serial.println();
}

void elapsedTime()
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

    M5.Lcd.setCursor(10, 30);
    M5.Lcd.print(days, DEC);
    M5.Lcd.print(" Days|");
    M5.Lcd.print(" ");
    M5.Lcd.print(hours, DEC);
    M5.Lcd.print(" Hours|");
    M5.Lcd.print(" ");
    M5.Lcd.print(minutes, DEC);
    M5.Lcd.print(" Minutes|");
    M5.Lcd.print(" ");
    M5.Lcd.print(seconds, DEC);
    M5.Lcd.print(" Seconds");

  } // end 1 second
}

int recvWithStartEndMarkers() // function to read from serial Receive with start- and end-markers
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  // record the current time using millis()
  int t_start = millis();

  while (Serial1.available() > 0 && newData == false)
  {
    // check to see if we timed out return 0 if all is ok return 1 if timeout occured
    if (millis() > (t_start + timeout_ms))
    {
      M5.Lcd.setCursor(10, 90);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.print("<M5 timedout and needs a reboot>");
      Serial.print("<M5 timedout and needs a reboot>");
      Serial1.write("<M5 timedout and needs a reboot>");
      // Serial1.flush();
      return 1;
    }

    rc = Serial1.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
  return 0;
}

void showNewData()
{
  if (newData)
  {
    Serial.print("Recieved ... ");
    Serial.println(receivedChars);

    if (!strcmp(receivedChars, "11"))
    { // if <11> is sent: both relays on
      M5.Lcd.setCursor(10, 70);
      M5.Lcd.print("RELAY_1 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 80);
      M5.Lcd.print("RELAY_2 STATE IS NOW:  ON");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      Serial1.write("<11>"); // send a message back that shows the current state of the relay
                             // Serial1.flush();
      Serial.println("RELAY_1 AND RELAY_2 STATE IS NOW: ON");
    }

    if (!strcmp(receivedChars, "00"))
    { // if <00> is sent: both relays off
      M5.Lcd.setCursor(10, 70);
      M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
      M5.Lcd.setCursor(10, 80);
      M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      Serial1.write("<00>"); // send a message back that shows the current state of the relay
                             // Serial1.flush();
      Serial.println("RELAY_1 AND RELAY_2 STATE IS NOW: OFF");
    }

    if (!strcmp(receivedChars, "01"))
    { // if <01> is sent: relay 2 is off and relay 1 is on
      M5.Lcd.setCursor(10, 70);
      M5.Lcd.print("RELAY_1 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 80);
      M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, LOW);
      Serial1.write("<01>"); // send a message back that shows the current state of the relay
                             // Serial1.flush();
      Serial.println("RELAY_1 STATE IS NOW: ON");
    }

    if (!strcmp(receivedChars, "10"))
    { // if <10> is sent: relay 2 is on and relay 1 is off
      M5.Lcd.setCursor(10, 70);
      M5.Lcd.print("RELAY_2 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 80);
      M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_1, LOW);
      Serial1.write("<10>"); // send a message back that shows the current state of the relay
                             // Serial1.flush();
      Serial.println("RELAY_2 STATE IS NOW: ON");
    }

    if (!strcmp(receivedChars, "f"))
    { // if <f> is sent: flusg Serial1 Write
      Serial1.flush();
    }

    newData = false;
  }
}

void loop()
{
  elapsedTime();
  recvWithStartEndMarkers();
  showNewData();
}
