#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <servoMotor.h>
#include <drive.h>
#include <uzd.h>

void moveToTakeObjectOnBlack()
{
  driveToObjectOnBlack();
  go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
  closeServo();        // закрываем сервопривод
  driveBackToCross();
  doezd();

  // while (!isOnCross()) // Едем вперед пока не доедем до перекрестка
  // {
  //   preg(baseSpeed);
  // }
  // go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
}

void moveToPutObjectOnBlack()
{
  // M_sensor();
  pregSomeTime(timeToMoveBanka);

  go(0, 0, baseDelay);
  openServo();
  driveBackToCross();
  doezd();
}

void moveObjectNextCross()
{
  go(baseSpeed, -baseSpeed, povorotDelay);
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
  right();
  right();
  moveToPutObjectOnBlack();
  left();
}

void M_sensor()
{
  while (isOnBlack(IR_SENSOR_M_PIN))
  {
    preg(baseSpeed);
  }
}

void obezdObject()
{
  if (uzdF() < distanceToCheckObject)
  {
    go(0, 0, baseDelay);
    go(baseSpeed, -baseSpeed, povorotDelay); // поворачиваем влево пока мы на линии с банкой
    go(0, 0, baseDelay);
    go(baseSpeed, baseSpeed, obezdDelay * 2);      // едем вперед секунду чтобы уйти с линии где банка
    go(-baseSpeed, baseSpeed, povorotDelay);       // поворачиваем вправа
    go(baseSpeed, baseSpeed, obezdObjectDelay*1.5);    // едем вперед чтобы обехать банку
    go(-baseSpeed, baseSpeed, povorotDelay / 1.5); // поворачиваем вправо
    while (IR_SENSOR_R_PIN > blackLimit)           // едем пока не вернемся на линию
    {
      go(baseSpeed, baseSpeed);
    }
  }
}
// перемещение банки на другую сторону перекрестка
void perekrestok()
{
  // crossCount++;
  // finish();
  // doezd();
  left();

  if (uzdF() < distanceToCheckBanka)
  {
    //  moveBankaTake();
    // moveBankaPut();
    right();
    right();
    while (uzdF() > distanceToTakeBanka)
    {
      preg(baseSpeed);
    }
    go(baseSpeed, baseSpeed, baseDelay);
    go(0, 0, baseDelay);
    go(-baseSpeed, -baseSpeed, baseDelay);
    left();
    while (!isOnCross())
    {
      preg(baseSpeed);
    }
    doezd();
    right();
  }
  else
  {
    right();
  }
}