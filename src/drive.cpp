

#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <UZD.h>
#include <servoMotor.h>
#include <LCD.h>
#include <banka.h>
#include <encoder.h>

void go(int L, int R, int interval = 0)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW); // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(L));                  // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW); // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(R));                  // Управляем скоростью правого мотора

  // LCDprint(0, 5, L);
  // LCDprint(1, 5, R);

  delay(interval);
}
void preg(int speed)
{
  // float p_gain = (speed == 0) ? KOEF_ERROR * gainCoeff : KOEF_ERROR;

  int E = getIRError(); /**/

  int uprvozd = E * koef_preg_p;

  int M1 = speed + uprvozd;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed - uprvozd;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  //   LCDprint(0, 0, M1);
  //   LCDprint(1, 0, M2);
  //   LCDprint(1, 8, E);
  //   LCDprint(1, 8, uprvozd);
  //   delay(500);
  // lcdclear();

  go(M1, M2, 0);
}

void whitePreg(int speed)
{
  // float p_gain = (speed == 0) ? KOEF_ERROR * gainCoeff : KOEF_ERROR;

  int E = getIRError(); /**/

  int uprvozd = E * koef_preg_p;

  int M1 = speed - uprvozd;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed + uprvozd;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  //   LCDprint(0, 0, M1);
  //   LCDprint(1, 0, M2);
  //   LCDprint(1, 8, E);
  //   LCDprint(1, 8, uprvozd);
  //   delay(500);
  // lcdclear();

  go(M1, M2, 0);
}

void pid(int speed)
{

  if (maze==true) // коэффициенты для езды в лабиринте
  {
    Kp = 3;    // пропорциональный
    Ki = 0.0; // интегральный
    Kd = 1;    // диференциальный
  }
  else
  {
    if (driveForward) // коэффициенты для езды по прамой
    {
      Kp = 1; // пропорциональный
      Ki = 0; // интегральный
      Kd = 0; // диференциальный
    }
    else // коэффициенты для езды по черной и белой линии
    {
      Kp = 0.6; // пропорциональный
      Ki = 0;   // интегральный
      Kd = 0;   // диференциальный
    }
  }

  Ep = getError();

  float Ed = Ep - Ei;
  Ei = Ei * 0.7 + Ep * 0.3;

  //   Ei = Ei + Ep;
  // if (Ei > MAX_INTEGRAL) {
  //   Ei = MAX_INTEGRAL;
  // } else if (Ei < -MAX_INTEGRAL) {
  //   Ei = -MAX_INTEGRAL;
  // }

  float U = Ep * Kp + Ei * Ki + Ed * Kd;
  //  Serial.print(" ");
  //  Serial.print(Ep);
  //  Serial.print(" ");
  //  Serial.print(Ei);   
  //  Serial.print(" ");
  //  Serial.print(Ed);
  //  Serial.print(" ");
  //  Serial.println(U);

  int M1 = speed + U;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed - U;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  go(M1, M2);
}

void pidEnc(int Speed,float k){
int E=abs(getEncoder2())-abs(getEncoder1());
float M1=Speed+E*k; M1=constrain(M1,-250,250);
float M2=Speed-E*k; M2=constrain(M2,-250,250);

go(M1,M2);
if(abs(getEncoder1())>1000 || abs(getEncoder2())>1000) {clearEncoder1();clearEncoder2();}
}

void driveSomeDistance(int distance)
{
  driveForward = true;
  clearAllEncoders();
  while (abs(getEncoder1()) < distance && abs(getEncoder2()) < distance) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    pid(baseSpeed);
  }
  clearAllEncoders();
  go(0, 0);
  driveForward = false;
}

void turnEnc(int speed, int angle)
{
  int direction;
  Kp = 0.2; // пропорциональный

  int A = map(abs(angle), 0, 360, 0, 6000);

  if (angle > 0)
    direction = 1; // лево
  else
    direction = -1; // право

  while (abs(getEncoder1()) < A || abs(getEncoder2()) < A)
  {
    float E = getEncoderError();

    // Serial.print(" ");    
    // Serial.println(E);

    float U = E * Kp; 
    
    int M1 = direction * (speed + U);
    M1 =  constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    int M2 = -direction * (speed - U);
    M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    go(M1, M2);
  }
  clearAllEncoders();
  go(0, 0);
}



void start()
{
  // Едем вперед пока не увидим поперечную черную линию
  driveSomeDistance(START_WIDTH);  
  go(0, 0);
 
}

void finish()
{
  // if (crossCount == FINISH_CROSS_COUNT)
  //{
  go(baseSpeed, baseSpeed, finishDelay);
  go(0, 0);
  while (true)
    ;
  //}
}
void doezd()
{
  driveSomeDistance(CROSS_WIDTH);  
  go(0, 0);
}

void stop()
{
  for (int i = 0; i < 40; i++)
  {
    go(baseSpeed, baseSpeed, 1);
    go(-baseSpeed, -baseSpeed, 1);
  }
  go(0, 0);
}

void driveToObjectOnBlack()
{
  while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToTakeBanka) // едем вперед на preg() пока расстояние до банки не будет меньше  distanceToTakeBanka
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

void pregSomeTime(unsigned long timeToMove)
{
  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < timeToMove) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    preg(baseSpeed);
  }
  go(0, 0);
}


void fixPositionAfterTurn()
{
  // Выравниваем робота после поворота пока ошибка(разность показаний левого и правого ИК датчика) не будет меньше maxErrorTurnFix
  while (getIRError() < maxErrorTurnFix)
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
  go(-baseSpeed, baseSpeed, 400); // Поворачиваем так, чтобы левый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на черной линии
  {
    go(-baseSpeed, baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(-baseSpeed, baseSpeed, 200); // Поворачиваем так, чтобы левый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на белом поле
  {
    go(-baseSpeed, baseSpeed);
    // if (checkBanka()) // Если увидели банку останавливаем поворот
    // {
    //   go(0, 0, baseDelay);
    //   return; //Выход из функции
    // }
  }

  // fixPositionAfterTurn();

  go(0, 0);
}

void left()
{
  go(baseSpeed, -baseSpeed, povorotDelay); // Поворачиваем так, чтобы правый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на черной линии
  {
    go(baseSpeed, -baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(baseSpeed, -baseSpeed, povorotDelay2); // Поворачиваем так, чтобы правый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на белом поле
  {
    go(baseSpeed, -baseSpeed);
    // if (checkBanka()) // Если увидели банку останавливаем поворот
    // {
    //   go(0, 0, baseDelay);
    //   return; //Выход из функции
    // }
  }

  // fixPositionAfterTurn();

  go(0, 0);
}

void turnGCross()
{
  // если мы попали на резкий поворот то мы проверяем находятся ли правый и центральный датчик на черном и поворачиваем
  if (getIRSensorValue(IR_SENSOR_R_PIN) < blackEdgeLimit && getIRSensorValue(IR_SENSOR_L_PIN) > whiteEdgeLimit && crossCount == 8)
  {
    // doezd();
    right();
    pregSomeTime(300);
  }
  else if (getIRSensorValue(IR_SENSOR_R_PIN) > whiteEdgeLimit && getIRSensorValue(IR_SENSOR_L_PIN) < blackEdgeLimit && crossCount == 8)
  {
    // doezd();
    left();
    pregSomeTime(300);
  }
}

void MoveBankaCross()
{
  left();
  if (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) < distanceToCheckBanka)
  {
    while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToTakeBanka)
    {
      preg(baseSpeed);
    }
    go(0, 0, baseDelay);
    closeServo();
    right();
    while (!isOnCross())
    {
      preg(baseSpeed);
    }
    // doezd();
    pregSomeTime(2000);
    go(baseSpeed, baseSpeed, baseDelay);
    go(0, 0, baseDelay);
    openServo();
    go(-baseSpeed, -baseSpeed, baseDelay);
    go(0, 0, baseDelay);
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
    if (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) < distanceToCheckBanka)
    {
      while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToTakeBanka)
      {
        preg(baseSpeed);
      }
      go(0, 0, baseDelay);
      closeServo();
      right();
      while (!isOnCross())
      {
        preg(baseSpeed);
      }
      // doezd();
      pregSomeTime(2000);
      go(baseSpeed, baseSpeed, baseDelay);
      go(0, 0, baseDelay);
      openServo();
      go(-baseSpeed, -baseSpeed, baseDelay);
      go(0, 0, baseDelay);
      right();
      while (!isOnCross())
      {
        preg(baseSpeed);
      }
      left();
    }
  }
}

// void drive1()
// {
//   if (crossCount == 4)
//   {
//     right();
//   }
//   if (crossCount == 5)
//   {
//     right();
//   }
//   if (crossCount == 6)
//   {
//     right();
//   }
//   if (crossCount == 7)
//   {
//     moveToTakeObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 10)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 11)
//   {
//     right();
//   }
//   if (crossCount == 12)
//   {
//     left();
//   }
//   if (crossCount == 13)
//   {
//     right();
//   }
//   if (crossCount == 14)
//   {
//     while (getDistance(UZS_TRIGGER_PIN, UZS_ECHO_PIN) > distanceToCheckObject)
//     {
//       preg(baseSpeed);
//     }
//     go(baseSpeed, baseSpeed, baseDelay * 3);
//     go(0, 0, baseDelay);
//     while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToCheckObject)
//     {
//       go(baseSpeed, -baseSpeed);
//     }
//     go(0, 0, baseDelay);
//     moveToTakeObjectOnBlack();
//     left();
//   }
//   if (crossCount == 15)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 16)
//   {
//     right();
//   }
//   if (crossCount == 21)
//   {
//     finish();
//   }
// }

// void drive2()
// {
//   if (crossCount == 4)
//   {
//     right();
//   }
//   if (crossCount == 5)
//   {
//     right();
//   }
//   if (crossCount == 6)
//   {
//     right();
//   }
//   if (crossCount == 7)
//   {
//     moveToTakeObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 9)
//   {
//     left();
//   }
//   if (crossCount == 10)
//   {
//     right();
//   }
//   if (crossCount == 11)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 13)
//   {
//     right();
//   }

//   if (crossCount == 14)
//   {
//     while (getDistance(UZS_TRIGGER_PIN, UZS_ECHO_PIN) > distanceToCheckObject)
//     {
//       preg(baseSpeed);
//     }
//     go(baseSpeed, baseSpeed, baseDelay * 3);
//     go(0, 0, baseDelay);
//     while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToCheckObject)
//     {
//       go(baseSpeed, -baseSpeed);
//     }
//     go(0, 0, baseDelay);
//     moveToTakeObjectOnBlack();
//     left();
//   }
//   if (crossCount == 15)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 16)
//   {
//     right();
//   }
//   if (crossCount == 21)
//   {
//     finish();
//   }
// }

// void drive3()
// {
//   if (crossCount == 4)
//   {
//     right();
//   }
//   if (crossCount == 5)
//   {
//     right();
//   }
//   if (crossCount == 6)
//   {
//     right();
//   }
//   if (crossCount == 7)
//   {
//     moveToTakeObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 9)
//   {
//     left();
//   }

//   if (crossCount == 11)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 12)
//   {
//     right();
//   }
//   if (crossCount == 13)
//   {
//     right();
//   }

//   if (crossCount == 14)
//   {
//     while (getDistance(UZS_TRIGGER_PIN, UZS_ECHO_PIN) > distanceToCheckObject)
//     {
//       preg(baseSpeed);
//     }
//     go(baseSpeed, baseSpeed, baseDelay * 3);
//     go(0, 0, baseDelay);
//     while (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) > distanceToCheckObject)
//     {
//       go(baseSpeed, -baseSpeed);
//     }
//     go(0, 0, baseDelay);
//     moveToTakeObjectOnBlack();
//     left();
//   }
//   if (crossCount == 15)
//   {
//     moveToPutObjectOnBlack();
//     left();
//     left();
//   }
//   if (crossCount == 16)
//   {
//     right();
//   }
//   if (crossCount == 21)
//   {
//     finish();
//   }
// }