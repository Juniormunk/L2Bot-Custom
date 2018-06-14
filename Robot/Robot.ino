#include <MotorDriver.h>

MotorDriver motor;

unsigned long lastWatchDogInt;
unsigned long currentWatchDogInt;

void setup() 
{
  Serial.begin(9600);
  motor.begin();
}
//INFO
//Right is 1 Left is 0

//  Input Data 
//  Motor Speed = Left:Right
//  Motor Brake = BRAKE
//  Motor Brake = BRAKE (L|R)
//  WatchDog = WATCHDOG

void loop() 
{
    if (Serial.available() > 0) 
    {
      String incomingSignal = Serial.readString();
      if(incomingSignal.equals("BRAKE"))
      {
        //BRAKE
        Serial.println("BRAKE");
        motor.brake(0);
        motor.brake(1);
      }
      else if(incomingSignal.equals("BRAKE R"))
      {
        //BRAKE
        Serial.println("BRAKE");
        motor.brake(1);
      }
      else if(incomingSignal.equals("BRAKE L"))
      {
        //BRAKE
        Serial.println("BRAKE");
        motor.brake(0);
      }
      else
      {

          int colonSpace = incomingSignal.indexOf(':');
          if(colonSpace != -1)
          {
            int leftMotorSpeed = incomingSignal.substring(0,colonSpace).toInt();
            int rightMotorSpeed = incomingSignal.substring(colonSpace+1,incomingSignal.length()).toInt();
            motor.speed(0, leftMotorSpeed); 
            motor.speed(1, rightMotorSpeed); 
          }
          else
          {
            lastWatchDogInt = millis();
          }

      }
    }
    if (millis()-lastWatchDogInt>1000)
    {
      //Stop Motor
      motor.stop(1);
      motor.stop(0);
      Serial.println("STOP!!!!!!");
    }
}
