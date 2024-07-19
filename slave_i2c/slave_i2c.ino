#include <Wire.h>

void setup() {
    Serial.begin(9600);          // start serial for output
    Wire.begin(9);               // join I2C bus with address #8
    Wire.onReceive(receiveEvent); // register event
}

void loop() {
  // delay(2000); // use this to find the right address
  // for(int i=1;i<128;i++){
  //   Wire.begin(i);               // join I2C bus with address #8
  //   Wire.onReceive(receiveEvent); // register event
  //   Serial.print("address: ");          // new line for readability
  //   Serial.println(String(i));          // new line for readability
  //
  //   delay(1000); // wait for a second
  // }
  //
  // Serial.println("END");
  delay(100);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all bytes
    char c = Wire.read();    // receive byte as a character
    Serial.print((int)c);         // print the character
  }
  Serial.println();          // new line for readability
}
