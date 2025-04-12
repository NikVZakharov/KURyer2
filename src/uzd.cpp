#include <Arduino.h>
#include <header.h>

// Константы
const unsigned long UZD_TIMEOUT = 5000;    // таймаут для pulseIn в мкс
const float MAX_VALID_DISTANCE = 400.0;    // максимальное допустимое расстояние в см
const int BUFFER_SIZE = 5;                 // размер буфера для измерений
const float JUMP_THRESHOLD = 10.0;         // порог для определения скачка значений (см)

// Буферы для хранения измерений каждого датчика
float frontBuffer[BUFFER_SIZE] = {100.0, 100.0, 100.0, 100.0, 100.0};
float rightBuffer[BUFFER_SIZE] = {100.0, 100.0, 100.0, 100.0, 100.0};
float leftBuffer[BUFFER_SIZE] = {100.0, 100.0, 100.0, 100.0, 100.0};

// Индексы для циклического заполнения буферов
int frontIndex = 0;
int rightIndex = 0;
int leftIndex = 0;

// Отфильтрованные значения
float filteredFront = 100.0;
float filteredRight = 100.0;
float filteredLeft = 100.0;

// Временные метки последних измерений
unsigned long lastFrontTime = 0;
unsigned long lastRightTime = 0;
unsigned long lastLeftTime = 0;
const unsigned long MEASUREMENT_INTERVAL = 10; // интервал между измерениями (мс)

/**
 * Базовое измерение расстояния ультразвуковым датчиком
 */
float uzd(int triggerPin) {
  // Настраиваем пин и отправляем импульс
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Переключаем на вход для чтения
  pinMode(triggerPin, INPUT);
  
  // Измеряем время и рассчитываем расстояние
  unsigned long duration = pulseIn(triggerPin, HIGH, UZD_TIMEOUT);
  float distance = 0.01723 * duration;
  
  return distance;
}

/**
 * Проверка на валидность измерения
 */
bool isValidMeasurement(float distance) {
  return (distance > 0 && distance < MAX_VALID_DISTANCE);
}

/**
 * Добавление нового измерения в буфер и расчет среднего
 */
float addMeasurementToBuffer(float newValue, float* buffer, int& index, float& filtered) {
  // Проверяем на валидность
  if (!isValidMeasurement(newValue)) {
    return filtered;
  }
  
  // Проверка на резкие скачки
  if (abs(newValue - filtered) > JUMP_THRESHOLD) {
    // Плавно интегрируем скачки (малый вес нового значения)
    filtered = filtered * 0.7 + newValue * 0.3;
  } else {
    // Обычная фильтрация
    filtered = filtered * 0.5 + newValue * 0.5;
  }
  
  // Добавляем новое отфильтрованное значение в буфер
  buffer[index] = filtered;
  index = (index + 1) % BUFFER_SIZE;
  
  // Считаем среднее по всему буферу
  float sum = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    sum += buffer[i];
  }
  
  return sum / BUFFER_SIZE;
}

/**
 * Получение расстояния от переднего датчика
 */
float getFDistance() {
  unsigned long currentTime = millis();
  
  // Проверяем, нужно ли обновлять значение
  if (currentTime - lastFrontTime < MEASUREMENT_INTERVAL) {
    // Возвращаем последнее отфильтрованное значение
    return filteredFront;
  }
  
  // Обновляем время последнего измерения
  lastFrontTime = currentTime;
  
  // Получаем новое измерение
  float rawDistance = uzd(UZF_TRIGGER_PIN);
  
  // Добавляем в буфер и фильтруем
  filteredFront = addMeasurementToBuffer(rawDistance, frontBuffer, frontIndex, filteredFront);
  
  return filteredFront;
}

/**
 * Получение расстояния от правого датчика
 */
float getRDistance() {
  unsigned long currentTime = millis();
  
  // Проверяем, нужно ли обновлять значение
  if (currentTime - lastRightTime < MEASUREMENT_INTERVAL) {
    // Возвращаем последнее отфильтрованное значение
    return filteredRight;
  }
  
  // Обновляем время последнего измерения
  lastRightTime = currentTime;
  
  // Получаем новое измерение
  float rawDistance = uzd(UZR_TRIGGER_PIN);
  
  // Добавляем в буфер и фильтруем
  filteredRight = addMeasurementToBuffer(rawDistance, rightBuffer, rightIndex, filteredRight);
  
  return filteredRight;
}

/**
 * Получение расстояния от левого датчика
 */
float getLDistance() {
  unsigned long currentTime = millis();
  
  // Проверяем, нужно ли обновлять значение
  if (currentTime - lastLeftTime < MEASUREMENT_INTERVAL) {
    // Возвращаем последнее отфильтрованное значение
    return filteredLeft;
  }
  
  // Обновляем время последнего измерения
  lastLeftTime = currentTime;
  
  // Получаем новое измерение
  float rawDistance = uzd(UZL_TRIGGER_PIN);
  
  // Добавляем в буфер и фильтруем
  filteredLeft = addMeasurementToBuffer(rawDistance, leftBuffer, leftIndex, filteredLeft);
  
  return filteredLeft;
}

/**
 * Функция-обертка для обратной совместимости
 */
float getDistance(int triggerPin) {
  if (triggerPin == UZF_TRIGGER_PIN) {
    return getFDistance();
  } else if (triggerPin == UZR_TRIGGER_PIN) {
    return getRDistance();
  } else if (triggerPin == UZL_TRIGGER_PIN) {
    return getLDistance();
  }
  // Если передан другой пин, используем базовое измерение без фильтрации
  return uzd(triggerPin);
}

/**
 * Получение всех трех измерений одновременно
 */
void getAllDistances(float &front, float &right, float &left) {
  front = getFDistance();
  right = getRDistance();
  left = getLDistance();
}

