#include <Servo.h>

Servo panServo;
Servo tiltServo;

int PAN_PIN = 9;
int TILT_PIN = 10;

int orderedPanPos = 90;
int orderedTiltPos = 90;

int count = 0;

const byte buffSize = 32;
int buff[buffSize];

void setup() {
  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);

  Serial.setTimeout(50);
  Serial.begin(9600);
}

void parseSerialInput(String data, char separator, int& resPan, int& resTilt) {
  for (int i=0; i<data.length(); i++) {
    if (data.charAt(i) == separator) {
      resPan = data.substring(0, i).toInt();
      resTilt = data.substring(i+1, data.length()).toInt();
      break; 
    }
  }
}

void loop() {
  if (Serial.available()) {

    String str = Serial.readStringUntil(';');
    int panAngle = 0;
    int tiltAngle = 0;
    parseSerialInput(str, ':', panAngle, tiltAngle);

    if (panAngle > 10 && panAngle < 170) {
      panServo.write(panAngle);
    }
    if (tiltAngle > 10 && tiltAngle < 170) {
      tiltServo.write(tiltAngle);
    }

    Serial.println(panAngle);
    Serial.println(tiltAngle);
  

  /*
    int readInt = Serial.parseInt();
    //int readInt = atoi(readChar);

    if (readInt == 2) {
      orderedPanPos = buff[0];
      orderedTiltPos = buff[1];
      count = 0;

      panServo.write(orderedPanPos);
      tiltServo.write(orderedTiltPos);
    }
    else if (readInt > 10 && readInt < 170) {
      buff[count] = readInt;
      count += 1;
    }
    */

    //Serial.println(buff[0]);
  }
}
