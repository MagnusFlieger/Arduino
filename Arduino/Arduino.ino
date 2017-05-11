// This will be the code uploaded to the main control Arduino

// INCLUDES
#include <Servo.h>


Servo myservo;  // create servo object to control a servo

int pos = 90;    // variable to store the servo position
int ByteReceived;
int Change = 0;


void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  myservo.write(pos);
}


void loop() {
  if (Serial.read() != -1) {
    ByteReceived = Serial.read();
    Serial.print(ByteReceived);
  }
  myservo.write(ByteReceived);
  delay(1000);
  ByteReceived = 0;
}
