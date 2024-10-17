#include <Arduino.h>
#include <header.h>
#include <middleware.h>
//#include <algorithm> // Для std::sort

void printArray(const float *arr, int size)
{
  for (int i = 0; i < size; i++)
  {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

float uzd(int trigerPin, int echoPin)
{
  digitalWrite(trigerPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigerPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigerPin, 0);

  float s = 0.01723 * pulseIn(echoPin, 1);

  return s;
}

// Получение расстояния
float getDistance(int triggerPin, int echoPin)
{
  float distance = uzd(triggerPin, echoPin);

  float meanDistance;
  if (triggerPin == UZF_TRIGGER_PIN)
  {
    meanDistance = getMedianValue(distance, pastUZDFValue);
  }
  else
  {
    meanDistance = getMedianValue(distance, pastUZDSValue);
  }
   
 // printArray(pastUZDSValue, meanArraySize);

  return meanDistance;
}

