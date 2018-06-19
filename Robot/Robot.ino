#include <MotorDriver.h>
String incomingSignal;
MotorDriver motor;

unsigned long lastWatchDogInt;
unsigned long currentWatchDogInt;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10);
  motor.begin();

  pinMode(52, OUTPUT);
  digitalWrite(52, HIGH);
}
//INFO
//Right is 0 Left is 1

//  Input Data
//  Motor Speed = .Left:Right
//  Motor Brake = .BRAKE
//  Motor Brake = .BRAKE (L|R)
//  WatchDog = .WATCHDOG

//  Output Data
//  Encoder Feedback = ENC:ENC
//  On Target Feedback =

void loop()
{
  parseSerial();
}


void parseSerial()
{
  if (Serial.available() > 0)
  {
    incomingSignal = Serial.readStringUntil('\n');
    incomingSignal = incomingSignal.substring(incomingSignal.indexOf(".") + 1, incomingSignal.length());
    int colonSpace = incomingSignal.indexOf(':');
    updateWatchDog();
    
    if (colonSpace == -1)
    {
      if (incomingSignal.equals("BRAKE"))
      {
        brakeR();
        brakeL();
      }
      else if (incomingSignal.equals("BRAKE R"))
      {
        brakeR();
      }
      else if (incomingSignal.equals("BRAKE L"))
      {
        brakeL();
      }
    }
    else
    {
      int leftMotorSpeed = incomingSignal.substring(0, colonSpace).toInt();
      int rightMotorSpeed = incomingSignal.substring(colonSpace + 1, incomingSignal.length()).toInt();
      drive(leftMotorSpeed, rightMotorSpeed);
    }
  }
}

void updateWatchDog()
{
  lastWatchDogInt = millis();
  //    if (millis()-lastWatchDogInt>1000)
  //    {
  //      //Stop Motor
  //      motor.stop(1);
  //      motor.stop(0);
  //      Serial.println("STOP!!!!!!");
  //    }
}
void brakeR()
{
  motor.brake(0);
}
void brakeL()
{
  motor.brake(1);
}
void drive(int left, int right)
{
  motor.speed(1, left);
  motor.speed(0, right);
  if (left == 0)
  {
    motor.stop(1);
    motor.brake(1);
  }
  if (right == 0)
  {
    motor.stop(0);
    motor.brake(0);
  }

}
