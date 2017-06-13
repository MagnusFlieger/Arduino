// This will be the code uploaded to the main control Arduino

// INCLUDES
#include <Servo.h>

// SERVOS
Servo throttle;       // Speed
Servo rotatingFront;  // cylinder at the front
Servo rotatingBack;   // cylinder at the back
Servo leftRight;      // left-right
Servo upDown;         // up-down

// SENSORS
// Nothing here yet

// VARIABLES
// Default positions for servos
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

int ByteReceived;   // store byte recieved via serial

char statusReport = 'A'; // one-byte status report to controller

bool batteryLow = false;
bool batteryOk = true;
bool motorOk = true;
bool servosOk = true;
bool sensorsOk = true;
bool arduinoOk = true;

// CONSTANTS

const int LOOP_DELAY = 100;



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
  // Get information from the sensors
  // Nothing here yet

  // Process information from the sensors
  // Nothing here yet
  
  // Get information from Serial
  // First byte: Servo to write to
  // Second byte: Value to write
  //bool success = true;
//  int Read = Serial.read();
//  int newPosition = 0;
//  switch (Read) {
//    case 1:
//      // Throttle
//      newPosition = Serial.read();
//      pos1 = newPosition;
//      throttle.write(pos1);
//      break;
//    case 2:
//      // Rotating Front
//      newPosition = Serial.read();
//      pos2 = newPosition;
//      rotatingFront.write(pos2);
//      break;
//    case 3:
//      // Rotating Back
//      newPosition = Serial.read();
//      pos3 = newPosition;
//      rotatingBack.write(pos3);
//      break;
//    case 4:
//      // Left-Right
//      newPosition = Serial.read();
//      pos4 = newPosition;
//      leftRight.write(pos4);
//      break;
//    case 5:
//      // Up-Down
//      newPosition = Serial.read();
//      pos5 = newPosition;
//      upDown.write(pos5);
//      break;
//
//    default:
//      // if nothing else matches, do the default
//      // default is optional
//      break;
//  }

  // Get information from Serial

  // Check if the information is even complete/correct

  // Check if there are any commands to execute

  // Read servo inputs
  int Read = 0;
  Read = Serial.read();
  pos1 = Read;
  Read = Serial.read();
  pos2 = Read;
  Read = Serial.read();
  pos3 = Read;
  Read = Serial.read();
  pos4 = Read;
  Read = Serial.read();
  pos5 = Read;

  // Update Servo positions
  throttle.write(pos1);
  rotatingFront.write(pos2);
  rotatingBack.write(pos3);
  leftRight.write(pos4);
  upDown.write(pos5);

  // Send information to Serial
  Serial.write(statusReport);
  Serial.write(pos1);
  Serial.write(pos2);
  Serial.write(pos3);
  Serial.write(pos4);
  Serial.write(pos5);

  // Delay
  delay(LOOP_DELAY);
}
