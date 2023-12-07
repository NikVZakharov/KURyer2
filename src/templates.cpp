#include <Arduino.h>
#include <NewPing.h>

#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <log.h>
#include <test.h>

void obezdBanki()
{
  if (uzdF() < distanceToCheckBanka / 1.6)
  {
    go(0, 0, baseDelay);
    go(-baseSpeed, baseSpeed, povorotDelay); // поворачиваем влево пока мы на линии с банкой
    go(0, 0, baseDelay);
    go(baseSpeed, baseSpeed, obezdDelay);       // едем вперед секунду чтобы уйти с линии где банка
    go(baseSpeed, -baseSpeed, povorotDelay);    // поворачиваем вправа
    go(baseSpeed, baseSpeed, obezdDelay * 1.5); // едем вперед чтобы обехать банку
    go(baseSpeed, -baseSpeed, povorotDelay);    // поворачиваем вправо
    while (IR_SENSOR_L_PIN > blackLimit)        // едем пока не вернемся на линию
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
  right();

  if (uzdF() < distanceToCheckBanka)
  {
  //  moveBankaTake();
  // moveBankaPut();
    right();
  }
  else
  {
    right();
    right();
    if (uzdF() < distanceToCheckBanka)
    {
   //   moveBankaTake();
   //   moveBankaPut();
      left();
    }
    else
    {
      right();
    }
  }
}
