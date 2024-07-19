#include <Wire.h>

void setup() {
  Wire.begin(); // join I2C bus as master
}

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("Hello");       // send message
  Wire.endTransmission();    // stop transmitting

  delay(500); // wait for a second
}
