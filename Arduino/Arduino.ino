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
// Default positions for servos
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

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
  // Get information from Serial
  // First byte: Servo to write to
  // Second byte: Value to write
  //bool success = true;
  int Read = Serial.read();
  switch (Read) {
    case 1:
      // Throttle
      int newPosition = Serial.read()
      pos1 = newPosition;
      throttle.write(pos1);
      break;
    case 2:
      // Rotating Front
      int newPosition = Serial.read()
      pos2 = newPosition;
      rotatingFront.write(pos2);
      break;
    case 3:
      // Rotating Back
      int newPosition = Serial.read()
      pos3 = newPosition;
      rotatingBack.write(pos3);
      break;
    case 4:
      // Left-Right
      int newPosition = Serial.read()
      pos4 = newPosition;
      leftRight.write(pos4);
      break;
    case 5:
      // Up-Down
      int newPosition = Serial.read()
      pos5 = newPosition;
      upDown.write(pos);
      break;

    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

  // Send information to Serial
  
  // Delay
  delay(1000);
}
