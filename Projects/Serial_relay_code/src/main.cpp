#include <M5Stack.h>

// This M5Stack Core 2 script controls two relays serial data
// from the serial monitor, capturing the individual
// characters, then evaluating the contents them to see what
// ostate the relas should be in. Relay packet design is as follows:
// 	<00> : all relays off
//	<01> : relay 1 on
//	<10> : relay 2 on
//	<11> : both relays on

const int RELAY_1 = 21;  // the board pin, which connects to the IN pin of relay
const int RELAY_2 = 22;  // the board pin, which connects to the IN pin of relay

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // Configure serial port that will be read to control relays
  pinMode(RELAY_1, OUTPUT); // Set pin 21 to output mode.
  pinMode(RELAY_2, OUTPUT); // Set pin 22 to output mode.
  digitalWrite(RELAY_1, LOW); // Set relay 1 to off at startup
  digitalWrite(RELAY_2, LOW); // Set relay 2 to off at startup
  M5.Lcd.print("Relay Control with Serial RS-232: v2");
  M5.Lcd.drawLine(0, 10, 320, 10, WHITE);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.print("<M5 is ready for relay commands>");
  Serial1.write("<M5 is ready for relay commands>");
  Serial1.flush();
  Serial.print("<M5 is ready for relay commands>");
  Serial.println();
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("Recieved ... ");
    Serial.println(receivedChars);

    if (strcmp(receivedChars, "11") == 0) { // if <11> is sent turn RELAY 1 on
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW:  ON");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      Serial1.write("<11>"); // send a message back that shows the current state of the relay
      Serial1.flush();
      Serial.println("RELAY_1 AND RELAY_2 STATE IS NOW: ON");
    }

    if (strcmp(receivedChars, "00") == 0) 
    { // if <00> is sent: both relays off
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      Serial1.write("<00>"); // send a message back that shows the current state of the relay
      Serial1.flush();
      Serial.println("RELAY_1 AND RELAY_2 STATE IS NOW: OFF");
    }

    if (strcmp(receivedChars, "01") == 0)
    { // if <01> is sent: relay 2 is off and relay 1 is on
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, LOW);
      Serial1.write("<01>"); // send a message back that shows the current state of the relay
      Serial1.flush();
      Serial.println("RELAY_1 STATE IS NOW: ON");
    }

    if (strcmp(receivedChars, "10") == 0)
    { // if <10> is sent: relay 2 is on and relay 1 is off
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW:  ON");
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_1, LOW);
      Serial1.write("<10>"); // send a message back that shows the current state of the relay
      Serial1.flush();
      Serial.println("RELAY_2 STATE IS NOW: ON");
    }

    newData = false;
  }
}

void loop() {
  recvWithStartEndMarkers();
  showNewData();
}