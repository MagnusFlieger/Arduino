// This will be the code uploaded to the main control Arduino

// INCLUDES
#include <Servo.h>

// SERVOS
Servo throttle;
Servo rotatingFront;
Servo rotatingBack;
Servo leftRight;
Servo upDown;

int pos = 90;    // variable to store the servo position
int ByteReceived;
int Change = 0;


void setup() {
  throttle.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  throttle.write(pos);
}


void loop() {
  if (Serial.read() != -1) {
    ByteReceived = Serial.read();
    Serial.print(ByteReceived);
  }
  throttle.write(ByteReceived);
  delay(1000);
  ByteReceived = 0;
}
