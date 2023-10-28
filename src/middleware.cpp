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
        minIRL = min(minIRL, irValue);
        maxIRL = max(maxIRL, irValue);

        return irValue=map(irValue, minIRL, maxIRL, 0, 1000); // Преобразовываем значения датчиков
    }
    else
    {
        minIRR = min(minIRR, irValue);
        maxIRR = max(maxIRR, irValue);

        return irValue=map(irValue, minIRR, maxIRR, 0, 1000); // Преобразовываем значения датчиков
    };
}

void preg() {
  int d1 = getIRSensorValue(IR_SENSOR_L_PIN);
  int d2 = getIRSensorValue(IR_SENSOR_R_PIN);
  if (d1 < min1) min1 = d1;
  if (d2 < min2) min2 = d2;
  if (d1 > max1) max1 = d1;
  if (d2 > max2) max2 = d2;
  d1 = map(analogRead(A0), min1, max1, 0, 1000);
  d2 = map(analogRead(A1), min2, max2, 0, 1000);
  int E = d1 - d2;
  float K = 0.4;
  int M1 = baseSpeed + E * K; M1 = constrain(M1, -255, 255);
  int M2 = baseSpeed - E * K; M2 = constrain(M2, -255, 255);
  go(M1, M2);
}


