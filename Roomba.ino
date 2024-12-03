
#include <Servo.h>
Servo myServo;
const int FLPin = 4;
const int FLSpeedPin = 6;
const int FRPin = 2;
const int FRSpeedPin = 5;
const int trigPin = 12;
const int echoPin = 13;

long distance, a1, a2;
static long duration;
const int servoPin = A3;


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
  Serial.begin(9600);

}



void loop() 
{
  
  

  Serial.println(getDistance());
  avoid();
  
  
  
}

void avoid()
{
  
  distance = getDistance();
  

  if((distance < 20)&&(distance != 0))

  {
    stop();
    delay(1000);
    myServo.write(160);
    delay(500);
    a1= getDistance();
    delay(100);
    myServo.write(20);
    delay(500);
    a2= getDistance();
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

static long getDistance()
{
  digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH);
    long sensorDistance = (duration / 2) * 0.0344;
    return sensorDistance;
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

  delay(1000);
}