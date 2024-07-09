#include <SoftwareSerial.h>

SoftwareSerial bluetooth(8, 7);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Bluetooth Started");
}

void loop() {
  if (Serial.available()){
    char data = Serial.read();
    bluetooth.write(data);
  }

  if (bluetooth.available()){
    char data = bluetooth.read();
    Serial.write(data);
  }
}
