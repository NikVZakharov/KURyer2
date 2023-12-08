#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <uzd.h>

void go(int L, int R, int interval = 0, bool fixMotor = true)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW);                         // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(fixMotor ? L * KOEFF_FIX_MOTOR_L_SPEED :L));                                          // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW);                         // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(R)); // Управляем скоростью правого мотора

  // Serial.print("fixMotor: "); Serial.println(fixMotor);
  // Serial.print("L: "); Serial.print(abs(fixMotor ? L * KOEFF_FIX_MOTOR_L_SPEED :L)); Serial.print(" R: "); Serial.println(abs(R));
  // delay(2000);

  delay(interval);

}
void preg(int speed)
{
  float p_gain = (speed == 0) ? KOEF_ERROR * gainCoeff : KOEF_ERROR;

  int E = currentError();

  int M1 = speed + E * p_gain;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed - E * p_gain;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  // Отладочное сообщение (можно закомментировать при финальной сборке)
  // Serial.print("p_gain: "); Serial.print(p_gain); Serial.print(" E: "); Serial.println(E);
  // Serial.print("M1: "); Serial.print(M1); Serial.print(" M2: "); Serial.println(M2);
  
  go(M1, M2,0,false);

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

void pregSomeTime(unsigned long timeToMove)
{
  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < timeToMove) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    preg(baseSpeed);
  }
  go(0, 0, baseDelay);
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
    while (true)
    {
    }
  }
}
void doezd()
{
  go(baseSpeed, baseSpeed, crossDelay);
  go(0, 0, baseDelay / 2);
}

void driveToObjectOnBlack(){
    while (uzdF() > distanceToTakeBanka) // едем вперед на preg() пока расстояние до банки не будет меньше  distanceToTakeBanka
  {
    preg(baseSpeed);
    // go(baseSpeed, baseSpeed);
  }
}


//едем назад пока не увидим черную линию
void driveBackToCross(){
while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
}

void rightWith()
{
  go(baseSpeed, -baseSpeed, 500); // Поворачиваем так, чтобы левый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_M_PIN)) // Поворачиваем пока левый ИК датчик на черной линии
  {
    go(baseSpeed, -baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(baseSpeed, -baseSpeed, 300); // Поворачиваем так, чтобы левый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_M_PIN)) // Поворачиваем пока левый ИК датчик на белом поле
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

void leftWith()
{
  go(-baseSpeed, baseSpeed, 500); // Поворачиваем так, чтобы правый ИК датчик сместился с белого на черную линию
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_M_PIN)) // Поворачиваем пока правый ИК датчик на черной линии
  {
    go(-baseSpeed, baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  go(-baseSpeed, baseSpeed, 300); // Поворачиваем так, чтобы правый ИК датчик сместился с черной линии на белое поле
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_M_PIN)) // Поворачиваем пока правый ИК датчик на белом поле
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