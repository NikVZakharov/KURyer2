#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <uzd.h>
#include <LCD.h>
#include <encoder.h>

// Функция для нормализации значений, которые выдает ИК датчик
int getIRSensorValue(int sensor)
{
  int result;
  int irValue = analogRead(sensor); // Читаем значение датчика

  if (sensor == IR_SENSOR_L_PIN)
  {
    minIRL = min(minIRL, irValue); // Обновляем минимальные и максимальные значения
    maxIRL = max(maxIRL, irValue); // Обновляем минимальные и максимальные значения

    result = map(irValue, minIRL, maxIRL, 0, 1024); // Преобразовываем значения датчиков
  }
  else
  {
    minIRR = min(minIRR, irValue);                  // Обновляем минимальные и максимальные значения
    maxIRR = max(maxIRR, irValue);                  // Обновляем минимальные и максимальные значения
    result = map(irValue, minIRR, maxIRR, 0, 1024); // Преобразовываем значения датчиков
  };
  // return IS_IR_SENSORS_REVERS? 1024-result:result;
  return result;
}

// Функция возвращает значение ошибки между показаниями левого и правого ИК датчика
int getIRError()
{
  int x = getIRSensorValue(IR_SENSOR_L_PIN);
  int y = getIRSensorValue(IR_SENSOR_R_PIN);
  // LCDprint(0, 0, x);
  // LCDprint(1, 0, y);
  return x - y;
}

int getEncoderError()
{
  int x = abs(getEncoder1());
  int y = abs(getEncoder2());
  // LCDprint(0, 0, x);
  // LCDprint(1, 0, y);
  return y - x;
}

int getWallError()
{
  return getDistance(UZS_TRIGGER_PIN, UZS_ECHO_PIN, pastUZDSValue) - walldistance;
}

int getError(int distance)
{
  int result;
  if (maze == true)
  {
    result = getWallError();
  }
  else
  {
    if (distance > 0)
    {
      result = getEncoderError();
    }
    else
    {
      result = getIRError();
    }
  }
  return result;
}

bool isOnBlack(int sensor)
{
  bool result = false;
  if (getIRSensorValue(sensor) < blackLimit)
  {
    result = true;
  }
  return result;
}

// находятся ли датчики на черной линии
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
  if (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN, pastUZDFValue) < distanceToCheckBanka)
  {
    result = true;
  }
  haveBanka = result;
  return result;
}
