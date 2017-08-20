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

bool sensorReportingOn = false;   // stores whether the sensor reporting feature is on
bool flightRecorderOn = false;    // stores whether the Arduino should record flight data

bool batteryLow = false;
bool batteryOk = true;
bool motorOk = true;
bool servosOk = true;
bool sensorsOk = true;
bool arduinoOk = true;

// CONSTANTS

const int LOOP_DELAY = 100;       // Delay of the loop cycle in milliseconds

//   Pins
const int LED_PIN = 5;
const int THROTTLE_PIN = 9;
const int FRONT_PIN = 10;
const int BACK_PIN = 11;
const int LR_PIN = 12;
const int UD_PIN = 13;



// INITIALIZE
void setup() {
  // Set up all servos
  throttle.attach(THROTTLE_PIN);
  rotatingFront.attach(FRONT_PIN);
  rotatingBack.attach(BACK_PIN);
  leftRight.attach(LR_PIN);
  upDown.attach(UD_PIN);

  // Begin Serial
  Serial.begin(9600);

  // Write default values to the servos
  throttle.write(pos1);
  rotatingFront.write(pos2);
  rotatingBack.write(pos3);
  leftRight.write(pos4);
  upDown.write(pos5);

  // Set up LED
  // Nothing here yet
}

// UPDATE
void loop() {
  // Get information from the sensors
  // Nothing here yet

  // Process information from the sensors
  // Nothing here yet
  
  // Get information from Serial
  if( Serial.available() > 0 )
  {
    // Loop through all bytes available
    char Read = '0';
    for ( int currentByte = 0; currentByte < Serial.available(); currentByte++ )
    {
      Read = Serial.read();
      switch (Read) 
      {
        // CONTROL SERVOS
        case 'A':
          // Throttle
          pos1 = Serial.peek();
          throttle.write(pos1);
          break;
        case 'B':
          // Rotating Front
          pos2 = Serial.peek();
          rotatingFront.write(pos2);
          break;
        case 'C':
          // Rotating Back
          pos3 = Serial.peek();
          rotatingBack.write(pos3);
          break;
        case 'D':
          // Left-Right
          pos4 = Serial.peek();
          leftRight.write(pos4);
          break;
        case 'E':
          // Up-Down
          pos5 = Serial.peek();
          upDown.write(pos5);
          break;
        
        // OTHER COMMANDS
        case 'X':
          // Reset and initialize everything
          break;
        case 'S':
          // Turn on Self-stabilizing
          break;
        case 's':
          // Turn off Self-stabilizing
          break;
        case 'G':
          // Turn on Sensor
          break;
        case 'g':
          // Turn off Sensor
          break;
        case 'Z':
          // Emergency
          break;
        default:
          // if nothing else matches, do the default
          // default is optional
          break;
      }
    }
  }
  
  // Update Servo positions
  throttle.write(pos1);
  rotatingFront.write(pos2);
  rotatingBack.write(pos3);
  leftRight.write(pos4);
  upDown.write(pos5);

  // Send status to Serial
  Serial.write(statusReport);

  // Send sensor data to serial
  if ( sensorReportingOn )
  {
    // Nothing here yet
  }

  // Record flight data
  if ( flightRecorderOn )
  {
    // Nothing here yet
  }

  // Update LEDs
  // Nothing here yet

  // Delay
  delay(LOOP_DELAY);
}
