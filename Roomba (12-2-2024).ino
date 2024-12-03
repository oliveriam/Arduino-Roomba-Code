#include <HCSR04.h>
#include <Servo.h>
Servo myServo;
const int FLPin = 4;
const int FLSpeedPin = 6;
const int FRPin = 2;
const int FRSpeedPin = 5;
const int trigPin = 12;
const int echoPin = 13;
HCSR04 hcsr04 = HCSR04(echoPin, trigPin);
long distance, a1, a2;
const int servoPin = A3;
//const int BLPin = 7;
//const int BLSpeedPin = 10;
//const int BRPin = 8;
//const int BRSpeedPin = 9;

void setup() 
{
  pinMode(FLPin,OUTPUT);
  pinMode(FLSpeedPin,OUTPUT);
  pinMode(FRPin,OUTPUT);
  pinMode(FRSpeedPin,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(90);
  //pinMode(BLPin,OUTPUT);
  //pinMode(BLSpeedPin,OUTPUT);
  //pinMode(BRPin,OUTPUT);
  //pinMode(BRSpeedPin,OUTPUT);
  Serial.begin(9600);

}



void loop() 
{
  distance = hcsr04.dist();
  Serial.println(distance);
  //avoid();
  
  
  
}

void avoid()
{
  
  distance = hcsr04.dist();
  

  if((distance < 20)&&(distance != 0))

  {
    stop();
    delay(1000);
    myServo.write(160);
    delay(500);
    a1=hcsr04.dist();
    delay(100);
    myServo.write(20);
    delay(500);
    a2=hcsr04.dist();
    delay(100);
    myServo.write(90);
    delay(500);
    if(a1 > a2)
    {
      turnLeft();
      myServo.write(90);
      delay(700); 
    }
    else
    {
      turnRight();
      myServo.write(90);
      delay(700);
    }
  }
  else
  {
    driveForward();
  }
}


void driveForward()
{
  digitalWrite(FLPin,HIGH);
  analogWrite(FLSpeedPin,100);  
  digitalWrite(FRPin,HIGH);
  analogWrite(FRSpeedPin,100);
  
  delay(2000);
}
void driveBackward()
{
    digitalWrite(FLPin, LOW);
  analogWrite(FLSpeedPin,100);  
  digitalWrite(FRPin,LOW);
  analogWrite(FRSpeedPin,100);
  
  delay(2000);
}

void turnLeft()
{
  digitalWrite(FLPin, LOW);
  analogWrite(FLSpeedPin,200);  
  digitalWrite(FRPin,HIGH);
  analogWrite(FRSpeedPin,200);
  delay(2000);
}

void turnRight()
{
  digitalWrite(FLPin, HIGH);
  analogWrite(FLSpeedPin,200);  
  digitalWrite(FRPin,LOW);
  analogWrite(FRSpeedPin,200);
  delay(2000);
} 
void stop()
{
  analogWrite(FRSpeedPin,0);
  analogWrite(FLSpeedPin,0);
  //analogWrite(BLSpeedPin,0);
  //analogWrite(BRSpeedPin,0);
  delay(1000);
}
