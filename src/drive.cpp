#include <Arduino.h>
#include <header.h>
#include <middleware.h>

void go(int L, int R, int interval = 0,bool fixMotor=true)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW); // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(L));                  // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW); // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(fixMotor?R*KOEFF_FIX_MOTOR_R_SPEED:R));                  // Управляем скоростью правого мотора

  delay(interval);
}
void preg(int speed)
{

  float p_gain;
  int E;
  speed == 0 ?: p_gain = KOEF_ERROR * gainCoeff, p_gain = KOEF_ERROR;

  // int d1 = analogRead(IR_SENSOR_L_PIN);
  // int d2 = analogRead(IR_SENSOR_R_PIN);

  // if (d1 < minIRL) minIRL = d1;
  // if (d2 < minIRR) minIRR = d2;
  // if (d1 > maxIRL) maxIRL = d1;
  // if (d2 > maxIRR) maxIRR = d2;
  // d1 = map(d1, minIRL, maxIRL, 0, 1000);
  // d2 = map(d2, minIRR, maxIRR, 0, 1000);

  E = currentError();

  int M1 = speed + E * p_gain;
  M1 = constrain(M1, -250, 250);
  int M2 = speed - E * p_gain;
  M2 = constrain(M2, -250, 250);
  go(M1, M2,0,false);
}


void right()
{
  go(baseSpeed, -baseSpeed, 500); // Поворачиваем так, чтобы левый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на черной линии
  {
    go(baseSpeed, -baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(baseSpeed, -baseSpeed, 300); // Поворачиваем так, чтобы левый ИК датчик сместился с черной линии на белое поле
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

  fixPositionAfterTurn();

  go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
}

void left()
{
  go(-baseSpeed, baseSpeed, 500); // Поворачиваем так, чтобы правый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на черной линии
  {
    go(-baseSpeed, baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(-baseSpeed, baseSpeed, 300); // Поворачиваем так, чтобы правый ИК датчик сместился с черной линии на белое поле
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

  fixPositionAfterTurn();

  go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
}

void pregSomeTime(int timeToMove)
{
  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < timeToMove) // Пока текущее время - время старта таймера меньше интервала выравнивания едем по preg()
  {
    preg(baseSpeed);
  }
  go(0,0,baseDelay);
}

void start()
{
  // Едем вперед пока не увидим поперечную черную линию
  while (!isOnCross())
  {
    go(baseSpeed, baseSpeed);
  }
  go(baseSpeed, baseSpeed, 300); // проезжаем поперечную черную линию пока черная линия трассы не окажется между датчиками
  go(0, 0);
}

void finish()
{
  if (crossCount == FINISH_CROSS_COUNT)
  {
    go(baseSpeed, baseSpeed, finishDelay * 4);
    go(0, 0);
    while (true)
    {
    };
  }
}
void doezd()
{
  go(baseSpeed, baseSpeed, crossDelay);
  go(0, 0, baseDelay / 2);
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
