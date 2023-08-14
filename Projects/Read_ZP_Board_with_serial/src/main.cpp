#include <Arduino.h>


#define RX_PIN 16  // Change to your desired RX pin
#define TX_PIN 17  // Change to your desired TX pin

HardwareSerial MySerial(1);  // UART1

void setup() {
  Serial.begin(115200);  // Start the built-in serial port
  MySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);  // Start the hardware serial port
  Serial.println("Time(sec)\tVoltage(mV)"); // Send the variable names
}

void loop() {
  if (MySerial.available()) {  // If data is available to read,
    String data = MySerial.readStringUntil('\n');  // read the entire line
    data.replace(',', '\t');  // replace the comma with a tab
    Serial.println(data);  // Print the modified data to the built-in serial port
  }
}



/*
void setup(){
    Serial.begin(115200);
}

void loop(){
    // get all of our variables of interest
    float t = millis()/1000.0;
    float var_sin = sin(t); // sin(t)
    float var_cos = cos(t); // cos(t)
    
    // write them all to console with tabs in between them
    Serial.print(t);         // first variable is program time in seconds. This can be plotted on an x-axis!
    Serial.print("\t");
    Serial.print(var_sin);   // second variable is sin(t)
    Serial.print("\t");      // this last "\t" isn't required, but doesn't hurt
    Serial.println(var_cos); // third varible is cos(t). make sure to finish with a println!
    
    // For example, at 2.5 seconds, this prints out like so, where \t
    // is the tab character, and \n is the newline character
    // 2500\t0.598\t-0.801\t\n
}
*/