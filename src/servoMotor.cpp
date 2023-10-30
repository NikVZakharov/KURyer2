
#include <Arduino.h>
#include <header.h>
#include <Servo.h>

Servo servo;

void initServo(){
      servo.attach(SERVO_PIN);
  servo.write(servoOpenPosition);
}

void closeServo()
{
  for (int i = servoOpenPosition; i < servoClosePosition; i++)
  {
    servo.write(i);
    delay(20);
  }
}

void openServo()
{
  /*for (int i = servoClosePosition; i < servoOpenPosition; i++)
  {
    servo.write(i);
    delay(20);
  }*/
  servo.write(servoOpenPosition);
}