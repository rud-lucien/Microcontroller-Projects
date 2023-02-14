#include <M5Stack.h>

const int RELAY_1 = 21;  // the board pin, which connects to the IN pin of relay
const int RELAY_2 = 22;  // the board pin, which connects to the IN pin of relay

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  pinMode(RELAY_1, OUTPUT);  // Set pin 21 to output mode.
  pinMode(RELAY_2, OUTPUT);  // Set pin 22 to output mode.
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
  M5.Lcd.print("Relay Control with Serial RS-232: v1");
  M5.Lcd.setCursor(10, 40);
  Serial.print("<M5 is ready for commands>");
  Serial.println();
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

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
    Serial1.println(receivedChars);

    if (strcmp(receivedChars, "11") == 0) { // if <11> is sent turn RELAY 1 on
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW: ON");
      digitalWrite(RELAY_1, HIGH);
      //Serial1.write("<11>");
      Serial.println("RELAY_1 STATE IS NOW: ON");
    }

    if (strcmp(receivedChars, "01") == 0) { // if <01> is sent turn RELAY 1 off
      M5.Lcd.setCursor(10, 40);
      M5.Lcd.print("RELAY_1 STATE IS NOW: OFF");
      digitalWrite(RELAY_1, LOW);
      //Serial1.write("<01>");
      Serial.println("RELAY_1 STATE IS NOW: OFF");
    }

    if (strcmp(receivedChars, "12") == 0) { // if <12> is sent turn RELAY 2 on
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW: ON");
      digitalWrite(RELAY_2, HIGH);
      //Serial1.write("<12>");
      Serial.println("RELAY_2 STATE IS NOW: ON");
    }

    if (strcmp(receivedChars, "02") == 0) { // if <02> is sent turn RELAY 2 off
      M5.Lcd.setCursor(10, 50);
      M5.Lcd.print("RELAY_2 STATE IS NOW: OFF");
      digitalWrite(RELAY_2, LOW);
      //Serial1.write("<02>");
      Serial.println("RELAY_2 STATE IS NOW: OFF");
    }

    newData = false;
  }
}

void loop() {
  recvWithStartEndMarkers();
  showNewData();
}