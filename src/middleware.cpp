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
  // lcdShow(0, 0, x);
  // lcdShow(1, 0, y);
  //Serial.print(" ");
  //Serial.print(isOnBlack(IR_SENSOR_M_PIN));

  //return isOnBlack(IR_SENSOR_M_PIN) ? x - y : y-x ;
  return whiteLineModeEnabled ?  x-y : y - x;
}

int getEncoderError()
{
  int x = abs(getEncoderL());
  int y = abs(getEncoderR());
  // lcdShow(0, 0, x);
  // lcdShow(1, 0, y);
  // Serial.print(x);
  // Serial.print("  ");
  // Serial.println(y);
  float z = x - y;
  // if (driveDirect)
  // {
  //   return -z;
  // }
  // else
  // {
    // Serial.print(x);
    // Serial.print("  ");
    // Serial.print(y);
    return z;
 // }
  
  
}

int getWallError()
{
  return walldistance - getDistance(UZR_TRIGGER_PIN);
}

extern const int WHITE_LINE_THRESHOLD;
extern bool whiteLineModeEnabled;

// Detection for white line on black background
bool isOnWhite(int sensor)
{
  int sensorValue = analogRead(sensor);
  return (sensorValue > WHITE_LINE_THRESHOLD);
}

// Check for white line crossing
bool isOnCrossWhite()
{
  bool result = false;
  if (isOnWhite(IR_SENSOR_L_PIN) && isOnWhite(IR_SENSOR_R_PIN))
  {
    result = true;
  }
  return result;
}

// Get IR error based on background type (white line mode)
int getIRErrorReversed()
{
  int x = getIRSensorValue(IR_SENSOR_L_PIN);
  int y = getIRSensorValue(IR_SENSOR_R_PIN);
  
  // For white line on black background, reverse the error calculation
  return x - y;
}

// Get error based on background type
int getError()
{
  int result;
  if (maze==true)
  {
    result = getWallError();
  }
  else
  {
    if (driveDirect==true)
    {
      result = getEncoderError();
    }
    else
    {
      if (whiteLineModeEnabled) {
        result = getIRErrorReversed();
      } else {
        result = getIRError();
      }
    }
  }
  return result;
}

// Check for intersection based on line color mode
bool isOnCross()
{
  if (whiteLineModeEnabled) {
    return isOnCrossWhite();
  } else {
    // Original black line crossing detection
    return (isOnBlack(IR_SENSOR_L_PIN) && isOnBlack(IR_SENSOR_R_PIN) && isOnBlack(IR_SENSOR_S_PIN));
  }
}

// bool checkBanka()
// {
//   bool result = false;
//   if (getDistance(UZF_TRIGGER_PIN, UZF_ECHO_PIN) < distanceToCheckBanka)
//   {
//     result = true;
//   }
//   haveBanka = result;
//   return result;
// }

void crossCalc()
{
  if(isOnCross())
  {
    crossCount++;
  }
}

// Function to check if robot has reached a new field
bool detectNewField() {
  // Read all three IR sensors
  int leftValue = analogRead(IR_SENSOR_L_PIN);
  int rightValue = analogRead(IR_SENSOR_R_PIN);
  int centerValue = analogRead(IR_SENSOR_S_PIN);
  
  // For a perpendicular line, all three sensors should detect black line
  bool allSensorsDetectLine = (leftValue < blackLimit) && 
                             (rightValue < blackLimit) && 
                             (centerValue < blackLimit);
  
  // To avoid false positives, we'll use a simple debounce
  static unsigned long lastDetectionTime = 0;
  static bool lastDetection = false;
  static int detectionCount = 0;
  
  if (allSensorsDetectLine) {
    // If this is a new detection (wasn't detected in previous call)
    if (!lastDetection) {
      lastDetection = true;
      lastDetectionTime = millis();
      detectionCount++;
      
      // For extra confidence, require multiple consecutive detections
      if (detectionCount >= 3) {
        detectionCount = 0;
        return true;
      }
    }
  } else {
    // Reset if no line detected for a while
    if (millis() - lastDetectionTime > 500) {
      lastDetection = false;
      detectionCount = 0;
    }
  }
  
  return false;
}