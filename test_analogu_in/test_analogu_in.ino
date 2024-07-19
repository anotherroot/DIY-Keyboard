// slave
#include <Wire.h>

void setup() {
  Wire.begin(4); // Join I2C bus with address 8 (you can choose any address between 1 and 127)
  Wire.onReceive(receiveEvent); // Register event handler for receiving data
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  // Main loop does nothing, waiting for data to be received
  delay(100);
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read(); // Read a byte from the master
    Serial.print(c); // Print the received byte to the Serial Monitor
  }
  Serial.println(); // Print a newline after each message
}

// #include <Wire.h>
//
// void setup() {
//   Wire.begin();
//   Serial.begin(9600);
//   while (!Serial); // Wait for serial monitor to open
//   Serial.println("\nI2C Scanner");
// }
//
// void loop() {
//   byte error, address;
//   int nDevices;
//
//   Serial.println("Scanning...");
//
//   nDevices = 0;
//   for (address = 1; address < 127; address++ ) {
//     // The i2c scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//
//     if (error == 0) {
//       Serial.print("I2C device found at address 0x");
//       if (address < 16) Serial.print("0");
//       Serial.print(address, HEX);
//       Serial.println("  !");
//
//       nDevices++;
//     } else if (error == 4) {
//       Serial.print("Unknown error at address 0x");
//       if (address < 16) Serial.print("0");
//       Serial.println(address, HEX);
//     }
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
//
//   delay(5000); // wait 5 seconds for next scan
// }
