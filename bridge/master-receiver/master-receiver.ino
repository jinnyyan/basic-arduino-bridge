#include <ELECHOUSE_CC1101.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo

int pos = 0; // variable to store the servo position
byte moving = (byte) 0; // variable to store whether servo is moving
byte opened = (byte) 0; // variable to store whether bridge is open (this reflects master)
const int n = 2; // size of buffer
//byte  buffer[2] = {0,0};
byte  buffer[1] = {moving};

void setup(){
  Serial.begin(9600); // begins for writing to serial monitor
  Serial.println("Responder Initating");

  ELECHOUSE_cc1101.Init(F_868);  // set frequency - F_433, F_868, F_965 MHz
  ELECHOUSE_cc1101.SetReceive();
  myservo.attach(9); // attaches the servo on pin 9 of the servo object
  initiatePosition(); // upon initiating, bridge should be closed at the 0 position
  }

void loop()
{
  if (ELECHOUSE_cc1101.CheckReceiveFlag())
  {
    int len = ELECHOUSE_cc1101.ReceiveData(buffer);
    moving = buffer[0];
//    opened = buffer[1];
    Serial.println(moving);
//    Serial.println(opened);
    if (moving == '0'){ // TODO: Check value of what moving char is
      movePosition();
      ELECHOUSE_cc1101.SetReceive();
    }
  }
}

void movePosition() {
  if (pos == 0 && opened == '1') {         // Bridge is closed, heading towards open
    while (pos < 90) {
      pos++;
      myservo.write(pos);
      delay(15);
    }
  } else if (pos == 90 && opened == '0') { // Bridge is open, heading towards closed
    while (pos > 0) {
      pos--;
      myservo.write(pos);
      delay(15);
    }
  }
}

void initiatePosition() {
  // Ensure servo is at position 0 at the beginning
  Serial.println("Initiating position");
  while (pos < 0) {
    pos++;
    myservo.write(pos);
    delay(15);
  }
  while (pos > 0) {
    pos--;
    myservo.write(pos);
    delay(15);
  }
}
