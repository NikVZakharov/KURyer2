#include <Arduino.h>
#include <header.h>
#include <drive.h>


// Функция для нормализации значений ИК датчика
int getIRSensorValue(int sensor)
{
    int irValue = analogRead(sensor); // Читаем значение датчика
                                      // Обновляем минимальные и максимальные значения
    if (sensor == IR_SENSOR_L_PIN)
    {
       // minIRL = min(minIRL, irValue);
        //maxIRL = max(maxIRL, irValue);
        if (irValue < minIRL) minIRL = irValue;
        if (irValue > maxIRL) maxIRL = irValue;

        return irValue=map(analogRead(sensor), minIRL, maxIRL, 0, 1000); // Преобразовываем значения датчиков
    }
    else
    {
       // minIRR = min(minIRR, irValue);
        //maxIRR = max(maxIRR, irValue);
        if (irValue < minIRR) minIRR = irValue;
        if (irValue > maxIRR) maxIRR = irValue;

        return irValue=map(analogRead(sensor), minIRR, maxIRR, 0, 1000); // Преобразовываем значения датчиков
    };
}

void preg() {
  int d1 = analogRead(IR_SENSOR_L_PIN);
  int d2 = analogRead(IR_SENSOR_R_PIN);

  if (d1 < minIRL) minIRL = d1;
  if (d2 < minIRR) minIRR = d2;
  if (d1 > maxIRL) maxIRL = d1;
  if (d2 > maxIRR) maxIRR = d2;
  d1 = map(d1, minIRL, maxIRL, 0, 1000);
  d2 = map(d2, minIRR, maxIRR, 0, 1000);

  int E = d1 - d2;
  float K = 0.4;
  int M1 = baseSpeed + E * K; M1 = constrain(M1, -255, 255);
  int M2 = baseSpeed - E * K; M2 = constrain(M2, -255, 255);

  go(M1, M2);
}

bool isOnBlack(int sensor)
{
  bool result = false;
  if (analogRead(sensor) < blackLimit)
  {
    result = true;
  }
  return result;
}

bool isOnCross()
{
  bool result = false;
  if (isOnBlack(IR_SENSOR_L_PIN) && isOnBlack(IR_SENSOR_R_PIN))
  {
    result = true;
  }
  return result;
}

