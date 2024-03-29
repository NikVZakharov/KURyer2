#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <uzd.h>
#include <LCD.h>

// Функция для нормализации значений, которые выдает ИК датчик
int getIRSensorValue(int sensor)
{
  int irValue = analogRead(sensor); // Читаем значение датчика

  if (sensor == IR_SENSOR_L_PIN)
  {
    minIRL = min(minIRL, irValue); // Обновляем минимальные и максимальные значения
    maxIRL = max(maxIRL, irValue); // Обновляем минимальные и максимальные значения

    return irValue = map(analogRead(sensor), minIRL, maxIRL, 0, 1000); // Преобразовываем значения датчиков
  }
  else
  {
    minIRR = min(minIRR, irValue); // Обновляем минимальные и максимальные значения
    maxIRR = max(maxIRR, irValue); // Обновляем минимальные и максимальные значения

    return irValue = map(analogRead(sensor), minIRR, maxIRR, 0, 1000); // Преобразовываем значения датчиков
  };
}

// Функция возвращает значение ошибки между показаниями левого и правого ИК датчика
int currentError()
{
int x=getIRSensorValue(IR_SENSOR_L_PIN);
int y=getIRSensorValue(IR_SENSOR_R_PIN);
//LCDprint(0, 0, x);
//LCDprint(1, 0, y);
  return x - y;
}


bool isOnBlack(int sensor)
{
  bool result = false;
  if (analogRead(sensor) < blackLimitCross)
  {
    result = true;
  }
  return result;
}

//находятся ли датчики на черной линии
bool isOnCross()
{
  bool result = false;
  if (isOnBlack(IR_SENSOR_L_PIN) && isOnBlack(IR_SENSOR_R_PIN))
  {
    result = true;
  }
  return result;
}

bool checkBanka()
{
  bool result = false;
  if (uzdF() < distanceToCheckBanka)
  {
    result = true;
  }
  haveBanka = result;
  return result;
}

