#include <Arduino.h>
#include <header.h>
#include <middleware.h>

void go(int L, int R, int interval = 0)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW); // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(L));                  // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW); // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(R));                  // Управляем скоростью правого мотора

  delay(interval);
}

void right()
{
  go(baseSpeed, -baseSpeed,500);
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_L_PIN))
  {
    go(baseSpeed, -baseSpeed);
  }
   // go(0, 0, baseDelay/3);
  go(baseSpeed, -baseSpeed,500);
  //  go(0, 0, baseDelay);
   while (!isOnBlack(IR_SENSOR_L_PIN))
  {
    go(baseSpeed, -baseSpeed);
  }

  go(0, 0, baseDelay/3);
  startTime = millis();
  while (millis() - startTime < timeToCorrectTurn)
  {
    preg();
  }
  go(0, 0, baseDelay/3);
  go(-baseSpeed, -baseSpeed, timeToCorrectTurn*2);
  go(0, 0, baseDelay);
}

void left()
{
  go(-baseSpeed, baseSpeed);
  delay(1000);
  // while (!isOnBlack(IR_SENSOR_L_PIN))
  //   go(-baseSpeed, baseSpeed);
  // while (isOnBlack(IR_SENSOR_L_PIN))
  //   go(-baseSpeed, baseSpeed);
  while (!isOnBlack(IR_SENSOR_R_PIN))
    go(-baseSpeed, baseSpeed);

  go(0, 0, baseDelay);
}
