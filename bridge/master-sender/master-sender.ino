#include <ELECHOUSE_CC1101.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
byte moving = (byte) 0; // variable to store whether servo is moving
byte opened = (byte) 0; // variable to store whether bridge is open
const int n = 2; // size of buffer
byte buffer[2];

void setup() {
  Serial.begin(9600); // begins for writing to serial monitor
  Serial.println("Transponder initiating");
  
  ELECHOUSE_cc1101.Init(F_868); // set frequency - F_433, F_868, F_965 MHz
  pinMode(4, INPUT);  // attaches the button on pin 4
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  initiatePosition(); // upon initiating, bridge should be closed at the 0 position
}

void loop() {
  int status = digitalRead(4); // status of button
  if (status == 0) { // when button is pressed down
    moving = (byte) 1;
    movePosition();
  } else if (status == 1) { // when button is released
    moving = false;
  }
}

void movePosition() {
//  buffer[0] = moving;
//  buffer[1] = opened;
//  ELECHOUSE_cc1101.SendData(buffer, 1);
  ELECHOUSE_cc1101.SendData(moving, 1);
  if (pos == 0){          // Bridge is closed, heading towards open
    while(pos < 90){
      pos++;
      myservo.write(pos);
      delay(15);
    }
    opened = (byte) 1;
  } else if (pos == 90) { // Bridge is open, heading towards closed
     while (pos > 0){
      pos--;
      myservo.write(pos);
      delay(15);
    }
    opened = (byte) 0;
  }
  loop();
}

void initiatePosition() {
  // Ensure servo is at position 0 at the beginning
  while(pos < 0){
      pos++;
      myservo.write(pos);
      delay(15);
  }
  while (pos > 0){
    pos--;
    myservo.write(pos);
    delay(15);
  }
}
