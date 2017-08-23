// This will be the code uploaded to the main control Arduino

// INCLUDES
#include <Servo.h>
#include <Wire.h>

// SERVOS
Servo throttle;       // Speed
Servo rotatingFront;  // cylinder at the front
Servo rotatingBack;   // cylinder at the back
Servo leftRight;      // left-right
Servo upDown;         // up-down

// SENSORS
long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

// VARIABLES
// Default positions for servos
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

//orientation of MPU

int prevX = 0;
int prevY = 0;
int prevZ = 0;

int ByteReceived;   // store byte recieved via serial

char statusReport = 'A'; // one-byte status report to controller

bool stabilizingOn = false;       // stores wheter self-stabilizing is on.
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
const int LED_PIN       = 5;
const int THROTTLE_PIN  = 9;
const int FRONT_PIN     = 10;
const int BACK_PIN      = 11;
const int LR_PIN        = 12;
const int UD_PIN        = 13;

//   Bytes
//    Recieving prefixes
const char THROTTLE_PREFIX          = 'A';
const char FRONT_PREFIX             = 'B';
const char BACK_PREFIX              = 'C';
const char LR_PREFIX                = 'D';
const char UD_PREFIX                = 'E';

//    Sending prefixes
const char MPU_X_PREFIX             = 'J';
const char MPU_Y_PREFIX             = 'K';
const char MPU_Z_PREFIX             = 'L';

//    Recieving commands
const char RESET_COMMAND            = 'X';
const char STABILIZING_ON_COMMAND   = 'S';
const char STABILIZING_OFF_COMMAND  = 's';
const char SENSORS_ON_COMMAND       = 'G';
const char SENSORS_OFF_COMMAND      = 'g';
const char FLIGHT_REC_ON_COMMAND    = 'R';
const char FLIGHT_REC_OFF_COMMAND   = 'r';
const char EMERGENCY_COMMAND        = 'Z';

//    Sending commands
const char STABILIZING_ON_CONFIRM   = 'H';
const char STABILIZING_OFF_CONFIRM  = 'h';
const char SENSORS_ON_CONFIRM       = 'I';
const char SENSORS_OFF_CONFIRM      = 'i';
const char FLIGHT_REC_ON_CONFIRM    = 'O';
const char FLIGHT_REC_OFF_CONFIRM   = 'o';


// INITIALIZE
void setup() {
  Serial.begin(9600);
  // Set up all servos
  throttle.attach(THROTTLE_PIN);
  rotatingFront.attach(FRONT_PIN);
  rotatingBack.attach(BACK_PIN);
  leftRight.attach(LR_PIN);
  upDown.attach(UD_PIN);

  // Set up MPU
  setupMPU();
  Serial.write("setupped");
  // Calibrate sensors
  calibrateMPU();
  Serial.write("calibrated");
  // Begin Serial
  //Serial.begin(9600);

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
  recordGyroData();
  //processGyroData();
  //Serial.write("processed");
  if(prevX - rotX > 5 || prevX - rotX < -5 || prevY - rotY > 5 || prevY - rotY < -5 || prevZ - rotZ > 5 || prevZ - rotZ < -5)
  {
    prevX = rotX;
    prevY = rotY;
    prevZ = rotZ;
    Serial.write(MPU_X_PREFIX);
    Serial.write(int(rotX));
    Serial.write(MPU_Y_PREFIX);
    Serial.write(int(rotY));
    Serial.write(MPU_Z_PREFIX);
    Serial.write(int(rotZ));
  }
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
        case THROTTLE_PREFIX:
          // Throttle
          pos1 = Serial.peek();
          throttle.write(pos1);
          break;
        case FRONT_PREFIX:
          // Rotating Front
          pos2 = Serial.peek();
          rotatingFront.write(pos2);
          break;
        case BACK_PREFIX:
          // Rotating Back
          pos3 = Serial.peek();
          rotatingBack.write(pos3);
          break;
        case LR_PREFIX:
          // Left-Right
          pos4 = Serial.peek();
          leftRight.write(pos4);
          break;
        case UD_PREFIX:
          // Up-Down
          pos5 = Serial.peek();
          upDown.write(pos5);
          break;
        
        // OTHER COMMANDS
        case RESET_COMMAND:
          // Reset and initialize everything
          break;
        case STABILIZING_ON_COMMAND:
          // Turn on Self-stabilizing
          stabilizingOn = true;
          Serial.write(STABILIZING_ON_CONFIRM);
          break;
        case STABILIZING_OFF_COMMAND:
          // Turn off Self-stabilizing
          stabilizingOn = false;
          Serial.write(STABILIZING_OFF_CONFIRM);
          break;
        case SENSORS_ON_COMMAND:
          // Turn on Sensor
          sensorReportingOn = true;
          Serial.write(SENSORS_ON_CONFIRM);
          break;
        case SENSORS_OFF_COMMAND:
          // Turn off Sensor
          sensorReportingOn = false;
          Serial.write(SENSORS_OFF_CONFIRM);
          break;
        
        case EMERGENCY_COMMAND:
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


void setupMPU(){
  Wire.beginTransmission(0b1101000); //Beginn Transmission to MPU I2C address
  Wire.write(0x6B); //open PWR_MGMT register
  Wire.write(0b00000000); //Deactivating SLEEP-MODE in PWR_MGMT
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B); //Open Gyro register
  Wire.write(0x00000000); //Setting the gyro scale +/- 250deg./s 
  Wire.endTransmission();
}

void calibrateMPU(){
  recordGyroData();
  prevX = gyroX;
  prevY = gyroY;
  prevZ = gyroZ;
}


void recordGyroData() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  //while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
  //Serial.write("processed");
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

