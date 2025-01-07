#include <Servo.h>
#include <MPU6050.h>
#include "Wire.h"
Servo myServo;
const int MPUAddress = 0x68;
int16_t gyroX, gyroY, gyroZ;
int16_t accelerometerX, accelerometerY, accelerometerZ;
int16_t temperature;
int16_t aXOffset = 0;
int16_t aYOffset = 343.00000;
int16_t aZOffset = 16726.00000;
int16_t gXOffset = -180.00000;
int16_t gYOffset = 18.00000;
int16_t gZOffset = 450.00000;
const int FLPin = 4;
const int FLSpeedPin = 6;
const int FRPin = 2;
const int FRSpeedPin = 5;
const int trigPin = 12;
const int echoPin = 13;

long distance, a1, a2;
static long duration;
const int servoPin = A3;
bool left = false;

char tmpStr[7];
char* convert_int16_to_str(int16_t i) {
  sprintf(tmpStr, "%6d", i);
  return tmpStr;
}

void initializeSensor()
{
  //
  // Perfrom full reset as per MPU-6000/MPU-6050 Register Map and Descriptions, Section 4.28, pages 40 to 41.
  //

  // performing full device reset, disables temperature sensor, disables SLEEP mode
  Wire.beginTransmission(0x68);  // Device address.
  Wire.write(0x6B);              // PWR_MGMT_1 register.
  Wire.write(0b10001000);        // DEVICE_RESET, TEMP_DIS.
  Wire.endTransmission();
  delay(100);                    // Wait for reset to complete.

  Wire.beginTransmission(0x68);  // Device address.
  Wire.write(0x68);              // SIGNAL_PATH_RESET register.
  Wire.write(0b00000111);        // GYRO_RESET, ACCEL_RESET, TEMP_RESET.
  Wire.endTransmission();
  delay(100);                    // Wait for reset to complete.

  // Disable SLEEP mode because the reset re-enables it. Section 3, PWR_MGMT_1 register, page 8.
  Wire.beginTransmission(MPUAddress);   // Device address.
  Wire.write(0x6B);                     // PWR_MGMT_1 register.
  Wire.write(0b00001000);               // SLEEP = 0, TEMP_DIS = 1.
  Wire.endTransmission();
}
void setup() {
  pinMode(FLPin, OUTPUT);
  pinMode(FLSpeedPin, OUTPUT);
  pinMode(FRPin, OUTPUT);
  pinMode(FRSpeedPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(50);
  Serial.begin(9600);
  Wire.begin();
  initializeSensor();
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
  accelerometerX = accelerometerX - aXOffset;
  accelerometerY = accelerometerY - aYOffset;
  accelerometerZ = accelerometerZ - aZOffset;
  gyroX = gyroX - gXOffset;
  gyroY = gyroY - gYOffset;
  gyroZ = gyroZ - gZOffset;
  
  // Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometerX));
  // Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometerY));
  // Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometerZ));
  //  //the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  // Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  // Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyroX));
  // Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyroY));
  // Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyroZ));
  // Serial.println();

  delay(1);
  // Serial.println(getDistance());
   avoid();

  
  
}

void avoid() {

  distance = getDistance();


  if ((distance < 20) && (distance != 0))

  {
    stop();
    delay(1000);
    myServo.write(145);
    delay(500);
    a1 = getDistance();
    delay(100);
    
    turnRight();
    
    myServo.write(50);
    
    delay(700);
    
    turnLeft();
    a2 = getDistance();
    delay(100);
    myServo.write(50);
    delay(500);
    if (a1 > a2) {
      turnLeft();
      myServo.write(50);
      delay(700);
    } else {
      turnRight();
      myServo.write(50);
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
  delay(320);
  stop();
}

void turnRight() {
  digitalWrite(FLPin, HIGH);
  analogWrite(FLSpeedPin, 200);
  digitalWrite(FRPin, LOW);
  analogWrite(FRSpeedPin, 200);
  delay(320);
  stop();
}
void stop() {
  analogWrite(FRSpeedPin, 0);
  analogWrite(FLSpeedPin, 0);

  delay(1000);
}