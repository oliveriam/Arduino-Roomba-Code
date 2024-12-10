#include <Servo.h>
#include "Wire.h"
Servo myServo;
const int MPUAddress = 0x68;
int16_t gyroX, gyroY, gyroZ;
int16_t accelerometerX, accelerometerY, accelerometerZ;
int16_t temperature;
const int FLPin = 4;
const int FLSpeedPin = 6;
const int FRPin = 2;
const int FRSpeedPin = 5;
const int trigPin = 12;
const int echoPin = 13;

long distance, a1, a2;
static long duration;
const int servoPin = A3;

char tmpStr[7];
char* convert_int16_to_str(int16_t i) {
  sprintf(tmpStr, "%6d", i);
  return tmpStr;
}


void setup() {
  pinMode(FLPin, OUTPUT);
  pinMode(FLSpeedPin, OUTPUT);
  pinMode(FRPin, OUTPUT);
  pinMode(FRSpeedPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  myServo.attach(servoPin);
  myServo.write(90);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPUAddress);
  Wire.write(0);
  Wire.endTransmission(true);
}



void loop() {
  Wire.beginTransmission(MPUAddress);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPUAddress, 7 * 2, true);
  accelerometerX = Wire.read() << 8 | Wire.read();
  accelerometerY = Wire.read() << 8 | Wire.read();
  accelerometerZ = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read();
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometerX));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometerY));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometerZ));
   //the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyroX));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyroY));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyroZ));
  Serial.println();

  delay(1000);
  //Serial.println(getDistance());
  //avoid();
}

void avoid() {

  distance = getDistance();


  if ((distance < 20) && (distance != 0))

  {
    stop();
    delay(1000);
    myServo.write(160);
    delay(500);
    a1 = getDistance();
    delay(100);
    myServo.write(20);
    delay(500);
    a2 = getDistance();
    delay(100);
    myServo.write(90);
    delay(500);
    if (a1 > a2) {
      turnLeft();
      myServo.write(90);
      delay(700);
    } else {
      turnRight();
      myServo.write(90);
      delay(700);
    }
  } else {
    driveForward();
  }
}

static long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  long sensorDistance = (duration / 2) * 0.0344;
  return sensorDistance;
}


void driveForward() {
  digitalWrite(FLPin, HIGH);
  analogWrite(FLSpeedPin, 100);
  digitalWrite(FRPin, HIGH);
  analogWrite(FRSpeedPin, 100);

  delay(100);
}
void driveBackward() {
  digitalWrite(FLPin, LOW);
  analogWrite(FLSpeedPin, 100);
  digitalWrite(FRPin, LOW);
  analogWrite(FRSpeedPin, 100);

  delay(2000);
}

void turnLeft() {
  digitalWrite(FLPin, LOW);
  analogWrite(FLSpeedPin, 200);
  digitalWrite(FRPin, HIGH);
  analogWrite(FRSpeedPin, 200);
  delay(100);
}

void turnRight() {
  digitalWrite(FLPin, HIGH);
  analogWrite(FLSpeedPin, 200);
  digitalWrite(FRPin, LOW);
  analogWrite(FRSpeedPin, 200);
  delay(100);
}
void stop() {
  analogWrite(FRSpeedPin, 0);
  analogWrite(FLSpeedPin, 0);

  delay(1000);
}
