#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <uzd.h>


// Функция для нормализации значений, которые выдает ИК датчик
int getIRSensorValue(int sensor)
{
    int irValue = analogRead(sensor); // Читаем значение датчика
                                      
    if (sensor == IR_SENSOR_L_PIN)
    {
        minIRL = min(minIRL, irValue);// Обновляем минимальные и максимальные значения
        maxIRL = max(maxIRL, irValue);// Обновляем минимальные и максимальные значения

        return irValue=map(analogRead(sensor), minIRL, maxIRL, 0, 1000); // Преобразовываем значения датчиков
    }
    else
    {
        minIRR = min(minIRR, irValue);// Обновляем минимальные и максимальные значения
        maxIRR = max(maxIRR, irValue);// Обновляем минимальные и максимальные значения
        
        return irValue=map(analogRead(sensor), minIRR, maxIRR, 0, 1000); // Преобразовываем значения датчиков
    };
}

//Функция возвращает значение ошибки между показаниями левого и правого ИК датчика
int currentError(){

  return getIRSensorValue(IR_SENSOR_L_PIN)-getIRSensorValue(IR_SENSOR_R_PIN);
}

void preg(int speed) {

  float p_gain;
  int E;
  speed==0?:p_gain=KOEF_ERROR*gainCoeff,p_gain=KOEF_ERROR;
  
  // int d1 = analogRead(IR_SENSOR_L_PIN);
  // int d2 = analogRead(IR_SENSOR_R_PIN);

  // if (d1 < minIRL) minIRL = d1;
  // if (d2 < minIRR) minIRR = d2;
  // if (d1 > maxIRL) maxIRL = d1;
  // if (d2 > maxIRR) maxIRR = d2;
  // d1 = map(d1, minIRL, maxIRL, 0, 1000);
  // d2 = map(d2, minIRR, maxIRR, 0, 1000);

  E = currentError();
  
  int M1 = speed + E * p_gain; M1 = constrain(M1, -250, 250);
  int M2 = speed - E * p_gain; M2 = constrain(M2, -250, 250);
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

bool checkBanka(){
    bool result = false;
  if (uzdF() < distanceToCheckBanka)
  {
    result = true; 
  }
  haveBanka=result;
  return result;
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

