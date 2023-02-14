#include <M5Stack.h>

// A very simple example of sending a string of characters
// from the serial monitor, capturing the individual
// characters into a String, then evaluating the contents
// of the String to perform an action (on/off board LED).

const int RELAY_PIN = 23; // the board pin, which connects to the IN pin of relay

String readString;

void setup()
{
  M5.begin();
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  M5.Lcd.setTextSize(1);
  digitalWrite(RELAY_PIN, LOW); // initialize relay pin to off
  pinMode(RELAY_PIN, OUTPUT);
  M5.Lcd.print("Relay Control with RS-232");
  Serial.println("serial on/off test 0021"); // so I can keep track
}

void receiveSerialData() // function to read string from serial and trigger relays
{
  while (Serial2.available()) // Don't read unless you know there is data
  {
    delay(3);
    char c = Serial2.read(); // Read a character
    readString += c;         // add chacracters togerthe and store it
  }

  if (readString.length() > 0)
  {
    Serial.println(readString);

    //////////////////

    if (readString.indexOf("on") >= 0)
    {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("RELAY ON"); // display on serial monitor 
      Serial2.write("RELAY ON"); // send a message back that shows the current state of the relay
    }

    if (readString.indexOf("off") >= 0)
    {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("RELAY OFF"); // display on serial monitor 
      Serial2.write("RELAY OFF"); // send a message back that shows the current state of the relay
    }

    //////////////////

    readString = ""; // Clear recieved buffer
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  receiveSerialData();
}

