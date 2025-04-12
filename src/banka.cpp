#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <servoMotor.h>
#include <drive.h>
#include <uzd.h>

void moveToTakeObjectOnBlack()
{
  driveToObjectOnBlack();
  closeServo();        // закрываем сервопривод
  // startTime = millis();                     // Считываем текущее время
  // while (millis() - startTime < baseDelay/2) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  // {
  //   go(-baseSpeed, -baseSpeed);
  // }
  driveBackToCross();
  doezd();
}

void moveToPutObjectOnBlack()
{
  // M_sensor();
  driveSomeTime(timeToMoveBanka);

  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < baseDelay) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    stop();
  }
  openServo();
  driveBackToCross();
  doezd();
}

void moveObjectNextCross()
{
  startTime = millis();                     // Считываем текущее время
  while ((millis() - startTime) < povorotDelay) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    go(baseSpeed, -baseSpeed);
  }
  while (distanceToTakeBanka > 10)
  {
    go(baseSpeed, baseSpeed);
  }
  moveToTakeObjectOnBlack();
  doezd();
  left();
}

void MoveObject90grad()
{
  moveToTakeObjectOnBlack();
  right();
  moveToPutObjectOnBlack();
  right();
}



void obezdObject(int distanceToStartObezd, int moveSide, int moveForward)
{
  while (getFDistance() > distanceToStartObezd)
  {
    pid(baseSpeed); // едем вперед пока не увидим банку
  }
  turnEnc(baseSpeed, -55);                // поворачиваем влево пока мы на линии с банкой
  driveSomeDistance(moveSide); // едем вперед, чтобы уйти с линии где банка
  turnEnc(baseSpeed, 55);
  driveSomeDistance(moveForward);       // едем вперед секунду чтобы уйти с линии где банка
  turnEnc(baseSpeed, 40);                          // поворачиваем влево
  while (IR_SENSOR_R_PIN > blackLimit) // едем пока не вернемся на линию
  {
    pid(baseSpeed);
  }
  stop();
}
