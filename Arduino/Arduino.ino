// This will be the code uploaded to the main control Arduino

// INCLUDES
#include <Servo.h>

// SERVOS
Servo throttle;
Servo rotatingFront;
Servo rotatingBack;
Servo leftRight;
Servo upDown;

// VARIABLES
int pos1 = 90;
int pos2 = 90;
int pos3 = 90;
int pos4 = 90;
int pos5 = 90;
int ByteReceived;   // store byte recieved via serial

// INITIALIZE
void setup() {
  // Set up all servos
  throttle.attach(9);
  rotatingFront.attach(10);
  rotatingBack.attach(11);
  leftRight.attach(12);
  upDown.attach(13);

  // Begin Serial
  Serial.begin(9600);

  // Write default values to the servos
  throttle.write(pos1);
  rotatingFront.write(pos2);
  rotatingBack.write(pos3);
  leftRight.write(pos4);
  upDown.write(pos5);
}

// UPDATE
void loop() {
  // Read information from Serial
  int Read = Serial.read();
  if (Read != -1) {
    ByteReceived = Read;
    Serial.print(ByteReceived);
  }

  // Write new positions
  throttle.write(ByteReceived);
  delay(1000);
  ByteReceived = 0;

  // Send information to Serial
  
}
