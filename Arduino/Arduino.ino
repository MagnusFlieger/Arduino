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

//orientation of MPU
int prevX = 0;
int prevY = 0;
int prevZ = 0;

// VARIABLES
// Default positions for servos
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;

int ByteReceived;   // store byte recieved via serial

bool stabilizingOn = false;       // stores wheter self-stabilizing is on.
bool sensorReportingOn = false;   // stores whether the sensor reporting feature is on
bool flightRecorderOn = false;    // stores whether the Arduino should record flight data

int iterationsSinceLastStatusReportSent = 0;

bool everythingOk = true;
bool batteryLow = false;
bool batteryFault = true;
bool motorFault = true;
bool servosFault = true;
bool sensorsFault = true;
bool arduinoFault = true;

// The individual LED states as an enum
enum ledStates {
  off,
  blinking,
  on
};

// The current LED state
ledStates ledState = off;

// CONSTANTS

const int LOOP_DELAY = 100;         // Delay of the loop cycle in milliseconds
const int LED_BLINK_INTERVAL = 500; // Blink interval in milliseconds

//   Pins
const int LED_PIN       = 5;
const int THROTTLE_PIN  = 9;
const int FRONT_PIN     = 10;
const int BACK_PIN      = 11;
const int LR_PIN        = 12;
const int UD_PIN        = 13;

//   Communication
//    Recieving prefixes
const char SPEED_PREFIX             = 'A';
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

//    Sending status reports
const char STATUS_OK                = 'A';
const char STATUS_MOTOR_FAULT       = 'C';
const char STATUS_SENSOR_FAULT      = 'D';
const char STATUS_FLIGHT_REC_FAULT  = 'E';
const char STATUS_ARDUINO_FAULT     = 'F';
const char STATUS_UNKNOWN_FAULT     = 'G';

const int ITERATIONS_FOR_EACH_STATUS_REPORT = 100;

//   MPU constants
const char MPU_POSITIVE_THRESHOLD   = 3;
const char MPU_NEGATIVE_THRESHOLD   = -3;


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
  // Calibrate sensors
  calibrateMPU();

  // Write default values to the servos
  throttle.write(pos1);
  rotatingFront.write(pos2);
  rotatingBack.write(pos3);
  leftRight.write(pos4);
  upDown.write(pos5);

  // Set up LED
  pinMode(LED_PIN, OUTPUT);
}

// UPDATE
void loop() {
  // Get information from the sensors
  recordGyroData();

  // Process information from the sensors
  // Nothing here yet
  
  // Get information from Serial
  if( Serial.available() > 0 )
  {
    // Loop through all bytes available
    char Read = '0';
    bool ignoreNextByte = false;
    for ( int currentByte = 0; currentByte < Serial.available(); currentByte++ )
    {
      Read = Serial.read();
      if (!ignoreNextByte)
      {
        switch (Read) 
        {
          // CONTROL SERVOS
        case SPEED_PREFIX:
            // Throttle
            pos1 = Serial.peek();
            throttle.write(pos1);
            ignoreNextByte = true;
            break;
          case FRONT_PREFIX:
            // Rotating Front
            pos2 = Serial.peek();
            rotatingFront.write(pos2);
            ignoreNextByte = true;
            break;
          case BACK_PREFIX:
            // Rotating Back
            pos3 = Serial.peek();
            rotatingBack.write(pos3);
            ignoreNextByte = true;
            break;
          case LR_PREFIX:
            // Left-Right
            pos4 = Serial.peek();
            leftRight.write(pos4);
            ignoreNextByte = true;
            break;
          case UD_PREFIX:
            // Up-Down
            pos5 = Serial.peek();
            upDown.write(pos5);
            ignoreNextByte = true;
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
        } // End switch
      } // End if !ignoreNextByte
      else
        ignoreNextByte = false;
    } // End for loop
  }

  // Send status to Serial
  if ( everythingOk )
  {
    if ( iterationsSinceLastStatusReportSent > ITERATIONS_FOR_EACH_STATUS_REPORT )
    {
      Serial.write(STATUS_OK);
      iterationsSinceLastStatusReportSent = 0;
    }
    else iterationsSinceLastStatusReportSent++;
  }
  else
  {
    if ( batteryFault )
    {
      Serial.write(STATUS_UNKNOWN_FAULT);
    }
    if ( motorFault )
      Serial.write(STATUS_MOTOR_FAULT);
    if ( servosFault )
      Serial.write(STATUS_UNKNOWN_FAULT);
    if ( sensorsFault )
      Serial.write(STATUS_SENSOR_FAULT);
    if ( arduinoFault )
      Serial.write(STATUS_ARDUINO_FAULT);
  }

  // Send sensor data to serial if we activated this
  if ( sensorReportingOn )
  {
    // Check if the values have changed much since the last iteration
    if( prevX - rotX > MPU_POSITIVE_THRESHOLD || prevX - rotX < MPU_NEGATIVE_THRESHOLD || 
        prevY - rotY > MPU_POSITIVE_THRESHOLD || prevY - rotY < MPU_NEGATIVE_THRESHOLD || 
        prevZ - rotZ > MPU_POSITIVE_THRESHOLD || prevZ - rotZ < MPU_NEGATIVE_THRESHOLD )
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
  }

  // Record flight data
  if ( flightRecorderOn )
  {
    // Nothing here yet
  }

  // Update LEDs
  switch (ledState)
  {
    case off:
      digitalWrite(LED_PIN, LOW);
    case on:
      digitalWrite(LED_PIN, HIGH);
    case blinking:
      break;
  }

  // Delay
  delay(LOOP_DELAY);
}

// Sets up the Gyro Sensor
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

// Calibrates the sensor
void calibrateMPU(){
  recordGyroData();
  prevX = gyroX;
  prevY = gyroY;
  prevZ = gyroZ;
}

// Records the data and processes it
void recordGyroData() {
  Wire.beginTransmission(0b1101000);  //I2C address of the MPU
  Wire.write(0x43);                   //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);      //Request Gyro Registers (43 - 48)
  //while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
  //Serial.write("processed");
}

// Processes the gyro values to rot
void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void RespondToPing()
{

}

void CalibrateMotor()
{
  // TODO
}

void ResetAndInitializeEverything()
{
  // TODO
}
