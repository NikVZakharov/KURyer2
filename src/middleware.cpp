#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <uzd.h>
#include <LCD.h>
#include <encoder.h>

float getMedianWithSort(const float *arr, int size)
{
  float tempArray[size];

  // Копируем исходный массив во временный массив
  for (int i = 0; i < size; i++)
  {
    tempArray[i] = arr[i];
  }

  // Сортировка временного массива методом пузырька
  float temp;
  for (int i = 0; i < size - 1; ++i)
  {
    for (int j = 0; j < size - i - 1; ++j)
    {
      if (tempArray[j] > tempArray[j + 1])
      {
        // Обмен элементов местами
        temp = tempArray[j];
        tempArray[j] = tempArray[j + 1];
        tempArray[j + 1] = temp;
      }
    }
  }

  //  printArray(tempArray, size); // Печать отсортированного временного массива

  // Нахождение медианы
  if (size % 2 == 1)
    return tempArray[size / 2];
  else
    return (tempArray[size / 2 - 1] + tempArray[size / 2]) / 2;
}

// Обновление массива и получение медианы
float getMedianValue(float currentValue, float *pastValues)
{
  // Сдвиг значений массива вправо
  for (int i = meanArraySize - 1; i > 0; i--)
  {
    pastValues[i] = pastValues[i - 1];
  }
  pastValues[0] = currentValue;

  return getMedianWithSort(pastValues, meanArraySize);
}

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
bool isOnBlack(int sensor)
{
  bool result = false;
  if (getIRSensorValue(sensor) < blackLimit)
  {
    result = true;
  }
  return result;
}
// Функция возвращает значение ошибки между показаниями левого и правого ИК датчика
int getIRError()
{

  int x = getIRSensorValue(IR_SENSOR_L_PIN);
  int y = getIRSensorValue(IR_SENSOR_R_PIN);
  // LCDprint(0, 0, x);
  // LCDprint(1, 0, y);
  //Serial.print(" ");
  //Serial.print(isOnBlack(IR_SENSOR_M_PIN));

  //return isOnBlack(IR_SENSOR_M_PIN) ? x - y : y-x ;
  return y - x;
}

int getEncoderError()
{
  int x = abs(getEncoder1());
  int y = abs(getEncoder2());
  // LCDprint(0, 0, x);
  // LCDprint(1, 0, y);
  // Serial.print(x);
  // Serial.print("  ");
  // Serial.println(y);

  return y - x;
}

int getWallError()
{
  return getDistance(UZS_TRIGGER_PIN, UZS_ECHO_PIN) - walldistance;
}

int getError()
{
  int result;
  if (maze==true)
  {
    result = getWallError();
  }
  else
  {
    if (driveForward==true)
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

// находятся ли датчики на черной линии
bool isOnCross()
{
  bool result = false;
  if (isOnBlack(IR_SENSOR_L_PIN) && isOnBlack(IR_SENSOR_R_PIN)/* && isOnBlack(IR_SENSOR_M_PIN)*/ /*|| !isOnBlack(IR_SENSOR_L_PIN) && !isOnBlack(IR_SENSOR_R_PIN)/* && !isOnBlack(IR_SENSOR_M_PIN)*/)
  {
    result = true;
    
  }
  return result;
}

bool checkBanka()
{
  bool result = false;
  if (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) < distanceToCheckBanka)
  {
    result = true;
  }
  haveBanka = result;
  return result;
}

void crossCalc()
{
  if(isOnCross())
  {
    crossCount++;
  }
}