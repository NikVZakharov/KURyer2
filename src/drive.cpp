

#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <uzd.h>
#include <servoMotor.h>
#include <LCD.h>
#include <encoder.h>


void go(int L, int R, int interval = 0)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW);                         // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(L)); // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW);                         // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(R));                                          // Управляем скоростью правого мотора

  //LCDprint(0, 5, L);
  //LCDprint(1, 5, R);

  delay(interval);
}
void preg(int speed)
{
  //float p_gain = (speed == 0) ? KOEF_ERROR * gainCoeff : KOEF_ERROR;
  float p_gain =koef_preg_p;
  int E = currentError();

  int uprvozd = E * koef_preg_p;

  int M1 = speed + uprvozd;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed - uprvozd;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  // LCDprint(0, 0, M1);
  // LCDprint(1, 0, M2);
  // LCDprint(1, 8, E);
  // LCDprint(1, 8, uprvozd);
  
  go(M1, M2, 0);
}


void encpid(int V, int K ,int N=0)
{
  int E = getEncoder1() - getEncoder2();

  int uprvozd = K * E;

  int M1=V + uprvozd;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2=V - uprvozd;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  go(M1, M2, 0);

  // LCDprint(0, 0, M1);
  // LCDprint(1, 0, M2);
  // LCDprint(1, 8, E);
  // LCDprint(1, 8, uprvozd);
  
  // if ((getEncoder1() >= N || getEncoder2() >= N) && N>0)
  // {
  //   go(0, 0);
  //   delay(2000);
  //   clearEncoder1();
  //   clearEncoder2();
  // }
}

void fixPositionAfterTurn()
{
  // Выравниваем робота после поворота пока ошибка(разность показаний левого и правого ИК датчика) не будет меньше maxErrorTurnFix
  while (currentError() < maxErrorTurnFix)
  {
    preg(0);
  }
  // if (fixPosition) // Корректируем положение машины относительно черной линии
  // {
  //   go(0, 0, baseDelay / 3);                         // Ждем пока закончится импульс инерции при повороте
  //   startTime = millis();                            // Считываем текущее время
  //   while (millis() - startTime < timeToMoveBanka) // Пока текущее время - время старта таймера меньше интервала выравнивания едем по preg()
  //   {
  //     preg();
  //   }
  //   go(0, 0, baseDelay / 3);                           // Ждем пока закончится импульс инерции
  //   go(-baseSpeed, -baseSpeed, timeToMoveBanka); // Едем назад,чтобы вернуться на перекресток
  // }
}

void right()
{
  go(baseSpeed, -baseSpeed, 400); // Поворачиваем так, чтобы левый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на черной линии
  {
    go(baseSpeed, -baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(baseSpeed, -baseSpeed, 200); // Поворачиваем так, чтобы левый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на белом поле
  {
    go(baseSpeed, -baseSpeed);
    // if (checkBanka()) // Если увидели банку останавливаем поворот
    // {
    //   go(0, 0, baseDelay);
    //   return; //Выход из функции
    // }
  }

   //fixPositionAfterTurn();

  go(0, 0);
}

void left()
{
  go(-baseSpeed, baseSpeed, povorotDelay); // Поворачиваем так, чтобы правый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на черной линии
  {
    go(-baseSpeed, baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(-baseSpeed, baseSpeed, povorotDelay2); // Поворачиваем так, чтобы правый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на белом поле
  {
    go(-baseSpeed, baseSpeed);
    // if (checkBanka()) // Если увидели банку останавливаем поворот
    // {
    //   go(0, 0, baseDelay);
    //   return; //Выход из функции
    // }
  }

   //fixPositionAfterTurn();

  go(0, 0); 
}

void pregSomeTime(unsigned long timeToMove)
{
  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < timeToMove) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    preg(baseSpeed);
  }
  go(0, 0);
}

void start()
{
  // Едем вперед пока не увидим поперечную черную линию
  while (!isOnCross())
  {
    go(baseSpeed, baseSpeed);
  }
  go(baseSpeed, baseSpeed, crossDelay); // проезжаем поперечную черную линию пока черная линия трассы не окажется между датчиками
  go(0, 0);
}

void finish()
{

  if (crossCount == FINISH_CROSS_COUNT)
  {
    go(baseSpeed, baseSpeed, finishDelay);
    go(0, 0);
    while (true);
  }
}
void doezd()
{
  go(baseSpeed, baseSpeed, crossDelay);
  go(0, 0, baseDelay );
}

void driveToObjectOnBlack()
{
  while (uzdF() > distanceToTakeBanka) // едем вперед на preg() пока расстояние до банки не будет меньше  distanceToTakeBanka
  {
    preg(baseSpeed);
    // go(baseSpeed, baseSpeed);
  }
}

// едем назад пока не увидим черную линию
void driveBackToCross()
{
  while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
}

void turnGCross()
{
  // если мы попали на резкий поворот то мы проверяем находятся ли правый и центральный датчик на черном и поворачиваем
  if (getIRSensorValue(IR_SENSOR_R_PIN) < blackEdgeLimit && getIRSensorValue(IR_SENSOR_L_PIN) > whiteEdgeLimit && crossCount==8)
  {
    doezd();
    right();
    pregSomeTime(300);
  }
  else  if (getIRSensorValue(IR_SENSOR_R_PIN) > whiteEdgeLimit && getIRSensorValue(IR_SENSOR_L_PIN) < blackEdgeLimit && crossCount==8)
  {
    doezd();
    left();
    pregSomeTime(300);
  }
}

void MoveBankaCross(){
  left();
  if (uzdF()<distanceToCheckBanka)
  {
    while (uzdF()>distanceToTakeBanka)
    {
      preg(baseSpeed);
    }
    go(0,0,baseDelay);
    closeServo();
    right();
    while (!isOnCross())
    {
     preg(baseSpeed);
    }
    doezd();
    pregSomeTime(2000);
    go(baseSpeed,baseSpeed,baseDelay);
    go(0,0,baseDelay);
    openServo();
    go(-baseSpeed,-baseSpeed,baseDelay);
    go(0,0,baseDelay);
    right();
    while (!isOnCross())
    {
     preg(baseSpeed);
    }
    right();
  }
  else  
  {
   right();
  if (uzdF()<distanceToCheckBanka)
  {
    while (uzdF()>distanceToTakeBanka)
    {
      preg(baseSpeed);
    }
    go(0,0,baseDelay);
    closeServo();
    right();
    while (!isOnCross())
    {
     preg(baseSpeed);
    }
    doezd();
    pregSomeTime(2000);
    go(baseSpeed,baseSpeed,baseDelay);
    go(0,0,baseDelay);
    openServo();
    go(-baseSpeed,-baseSpeed,baseDelay);
    go(0,0,baseDelay);
    right();
    while (!isOnCross())
    {
     preg(baseSpeed);
    }
    left();
  } 
}

}

