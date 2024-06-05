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
  moveToPutObjectOnBlack();
  right();
 
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
  if (getDistance(UZF_TRIGGER_PIN,UZF_ECHO_PIN,pastUZDFValue) < distanceToCheckObject)
  {
    go(0, 0, baseDelay);
    go(baseSpeed, -baseSpeed, povorotDelay); // поворачиваем влево пока мы на линии с банкой
    go(0, 0, baseDelay);
    go(baseSpeed, baseSpeed, obezdDelay);      // едем вперед секунду чтобы уйти с линии где банка
    go(-baseSpeed, baseSpeed, povorotDelay*2);       // поворачиваем влево
    while (IR_SENSOR_R_PIN > blackLimit)           // едем пока не вернемся на линию
    {
      go(baseSpeed, baseSpeed);
    }
  }
}
// перемещение банки на другую сторону перекрестка
void moveObjectFromCross()
{
  // crossCount++;
  // finish();
  // doezd();
  left();

  if (getDistance(UZF_TRIGGER_PIN,UZF_ECHO_PIN,pastUZDFValue) < distanceToCheckBanka)
  {
    //  moveBankaTake();
    // moveBankaPut();
    right();
    right();
    while (getDistance(UZF_TRIGGER_PIN,UZF_ECHO_PIN,pastUZDFValue) > distanceToTakeBanka)
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