#include <MotorDriver.h>

MotorDriver motor;

unsigned long lastWatchDogInt;
unsigned long currentWatchDogInt;

void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(10);
  motor.begin();
}
//INFO
//Right is 0 Left is 1

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
        motor.brake(1);
        motor.brake(0);
      }
      else if(incomingSignal.equals("BRAKE R"))
      {
        //BRAKE
        motor.brake(0);
      }
      else if(incomingSignal.equals("BRAKE L"))
      {
        //BRAKE
        motor.brake(1);
      }
      else
      {

          int colonSpace = incomingSignal.indexOf(':');
          if(colonSpace != -1)
          {
            int leftMotorSpeed = incomingSignal.substring(0,colonSpace).toInt();
            int rightMotorSpeed = incomingSignal.substring(colonSpace+1,incomingSignal.length()).toInt();
            motor.speed(1, leftMotorSpeed); 
            motor.speed(0, rightMotorSpeed); 
            if(leftMotorSpeed==0)
            {
              motor.stop(1);
              motor.brake(1);
            }
            if(rightMotorSpeed==0)
            {
              motor.stop(0);
              motor.brake(0);
            }
          }
          else
          {
            lastWatchDogInt = millis();
          }

      }
    }
//    if (millis()-lastWatchDogInt>1000)
//    {
//      //Stop Motor
//      motor.stop(1);
//      motor.stop(0);
//      Serial.println("STOP!!!!!!");
//    }
}
