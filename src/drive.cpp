#include <Arduino.h>
#include <header.h>
#include <middleware.h>
#include <UZD.h>
#include <servoMotor.h>
#include <LCD.h>
#include <banka.h>
#include <encoder.h>
#include <log.h>
#include <RGB.h>


// Parameters for obstacle avoidance
const int OBSTACLE_AVOIDANCE_SPEED = 170;   // Speed during obstacle avoidance
const int AVOID_TURN_DELAY = 500;           // Time to turn away from obstacle
const int AVOID_FORWARD_DELAY = 1000;       // Time to move forward during avoidance
const int AVOID_RETURN_DELAY = 500;         // Time to turn back to line
const int NEW_FIELD_THRESHOLD = 500;        // Threshold for perpendicular line detection


void go(int L, int R)
{
  digitalWrite(MOTOR_L_DIRECTION_PIN, L > 0 ? HIGH : LOW); // Управляем направлением левого мотора
  analogWrite(MOTOR_L_SPEED_PIN, abs(L));                  // Управляем скоростью левого мотора
  digitalWrite(MOTOR_R_DIRECTION_PIN, R > 0 ? HIGH : LOW); // Управляем направлением правого мотора
  analogWrite(MOTOR_R_SPEED_PIN, abs(R));                  // Управляем скоростью правого мотора

  // lcdShow(0, 5, L);
  // lcdShow(1, 5, R);
}

void pid(int speed)
{

  if (maze == true) // коэффициенты для езды в лабиринте
  {
    Kp = 3; // пропорциональный
    Ki = 0; // интегральный
    Kd = 1; // диференциальный
  }

  Ep = getError();

  float Ed = Ep - Ei;
  Ei = Ei * 0.7 + Ep * 0.3;

  //   Ei = Ei + Ep;
  // if (Ei > MAX_INTEGRAL) {
  //   Ei = MAX_INTEGRAL;
  // } else if (Ei < -MAX_INTEGRAL) {
  //   Ei = -MAX_INTEGRAL;
  // }

  float U = Ep * Kp + Ei * Ki + Ed * Kd;

  int M1 = speed - U;
  M1 = constrain(M1, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
  int M2 = speed + U;
  M2 = constrain(M2, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

  // lcdShow(0,0,M1);
  // lcdShow(1,0,M2);

  go(M1, M2);
}

void stop()
{
  // for (int i = 0; i < 40; i++)
  // {
  //   go(baseSpeed, baseSpeed);
  //   go(-baseSpeed, -baseSpeed);
  // }
  go(0, baseSpeed);
  delay(2000);
  go(0, 0);
}

void driveSomeDistance(int distance)
{
  // bool n = driveDirect;
  driveDirect = true;
  clearAllEncoders();
  while (abs(getEncoderL()) < distance && abs(getEncoderR()) < distance) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    pid(baseSpeed);
  }
  driveDirect = false;
  clearAllEncoders();
  stop();
}

void driveSomeTime(unsigned long timeToMove)
{
  driveDirect = true;
  clearAllEncoders();
  startTime = millis();                     // Считываем текущее время
  while (millis() - startTime < timeToMove) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    pid(baseSpeed);
  }
  driveDirect = false;
  clearAllEncoders();
  stop();
}

void turnEnc(int speed, int angle)
{
  int direction;
  Kp = 0.2; // пропорциональный

  // Очищаем энкодеры перед началом поворота
  clearAllEncoders();

  // Корректный масштаб для углов
  // 45 градусов ≈ 1500 тиков, 90 градусов ≈ 3000 тиков
  int A = map(abs(angle), 0, 360, 0, 8500);

  // Исправляем направление поворота
  if (angle > 0)
    direction = 1; // вправо (т.к. в функции go первый параметр - левый мотор)
  else
    direction = -1; // влево

  // Поворачиваем пока не достигнем нужного количества тиков
  while (abs(getEncoderL()) < A && abs(getEncoderR()) < A)
  {
    float E = getEncoderError();
    float U = E * Kp;

    // Направление движения моторов для поворота
    int ML = direction * (speed);
    int MR = -direction * (speed);

    // Корректируем значения с учетом ошибки энкодеров
    ML = ML + U * direction;
    MR = MR - U * direction;

    ML = constrain(ML, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
    MR = constrain(MR, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    go(ML, MR);
  }

  clearAllEncoders();
  stop();
}

void right()
{
  startTime = millis();              // Считываем текущее время
  while (millis() - startTime < 100) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    go(+baseSpeed, -baseSpeed); // Поворачиваем так, чтобы левый ИК датчик сместился с белого на черную линию
  }
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на черной линии
  {
    go(+baseSpeed, -baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  startTime = millis();              // Считываем текущее время
  while (millis() - startTime < 200) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    go(+baseSpeed, -baseSpeed); // Поворачиваем так, чтобы левый ИК датчик сместился с черной линии на белое поле
  }
  //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_L_PIN)) // Поворачиваем пока левый ИК датчик на белом поле
  {
    go(+baseSpeed, -baseSpeed);
  }

  // fixPositionAfterTurn();

  stop();
}

void left()
{
  startTime = millis();              // Считываем текущее время
  while (millis() - startTime < 100) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    go(-baseSpeed, +baseSpeed); // Поворачиваем так, чтобы правый ИК датчик сместился с белого на черную линию
  }
  //  go(0, 0, baseDelay);
  while (isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на черной линии
  {
    go(-baseSpeed, +baseSpeed);
  }
  // go(0, 0, baseDelay/3);
  startTime = millis();              // Считываем текущее время
  while (millis() - startTime < 200) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    go(-baseSpeed, +baseSpeed); // Поворачиваем так, чтобы правый ИК датчик сместился с черной линии на белое поле
  }
  // //  go(0, 0, baseDelay);
  while (!isOnBlack(IR_SENSOR_R_PIN)) // Поворачиваем пока правый ИК датчик на белом поле
  {
    go(-baseSpeed, +baseSpeed);
    // if (checkBanka()) // Если увидели банку останавливаем поворот
    // {
    //   go(0, 0, baseDelay);
    //   return; //Выход из функции
    // }
  }

  // fixPositionAfterTurn();

  stop();
}

void doezd()
{
  //driveSomeDistance(CROSS_WIDTH);
  startTime = millis();
  while (millis() - startTime < baseDelay)
  {
    go(baseSpeed,baseSpeed);
  }
  
  stop();
  // пауза для стабилизации  робота
  startTime = millis();
  while (millis() - startTime < baseDelay)
    ;
}

void start()
{
  go(baseSpeed,baseSpeed);
  delay(400);
  go(0,0);
  delay(1000);
  // Едем вперед пока не увидим поперечную черную линию
  // driveDirect = true;
  // clearAllEncoders();
  // while (!isOnCross()) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  // {
  //   pid(baseSpeed);
  // }
  // clearAllEncoders();
  // driveDirect = false;

  // driveSomeDistance(450);
  // stop();
  // // пауза для стабилизации  робота
  // startTime = millis();
  // while (millis() - startTime < baseDelay)
  //   ;
}

void finish()
{
  go(baseSpeed,baseSpeed);
  delay(500);
  go(0,0);
  //driveSomeDistance(finishDelay);
  while (true)
    ;
}



// Function for obstacle avoidance
void avoidObstacle() {
  
  lcdclear();
  lcdShow(0, 0, "Obstacle found!");
  lcdShow(1, 0, "Stopping");
  
  stop(); // Stop the robot
  delay(100);
  
  // Поворот влево для объезда препятствия (т.к. в go первый параметр - левый мотор)
  lcdclear();
  lcdShow(0, 0, "Turning left");
  lcdShow(1, 0, "45 degrees");
  
  turnEnc(OBSTACLE_AVOIDANCE_SPEED, -45);  // Поворот на 90 градусов влево

  lcdclear();
  lcdShow(0, 0, "Moving forward");
  lcdShow(1, 0, "Avoiding obstacle");
  
  driveSomeTime(AVOID_FORWARD_DELAY);
  
  // Поворот вправо для возврата к линии
  lcdclear();
  lcdShow(0, 0, "Turning right");
  lcdShow(1, 0, "90 degrees");
  
  turnEnc(OBSTACLE_AVOIDANCE_SPEED, 90);  // Поворот на 90 градусов вправо
  
  // Движение вперед до обнаружения линии
  lcdclear();
  lcdShow(0, 0, "Searching line");
  lcdShow(1, 0, "Moving forward");
  
  unsigned long searchStartTime = millis();
  const unsigned long MAX_SEARCH_TIME = 10000; // Максимальное время поиска линии
  
  driveDirect = true;
  while (millis() - searchStartTime < MAX_SEARCH_TIME) {
    pid(OBSTACLE_AVOIDANCE_SPEED/2);
    
    int leftValue = getIRSensorValue(IR_SENSOR_L_PIN);
    // int rightValue = getIRSensorValue(IR_SENSOR_R_PIN);
    
    // Если один из датчиков обнаружил линию
    if (leftValue < blackLimit) {
      stop();
      delay(50);
      isObezdDone = true;
      lcdclear();
      lcdShow(0, 0, "Line found!");
      lcdShow(1, 0, "Avoidance done");
      break;
    }
    
    // Периодическое обновление времени поиска
    if (millis() % 1000 < 50) {
      lcdShow(1, 0, "Time: ");
      lcdShow(1, 6, (millis() - searchStartTime) / 1000);
    }
    
    delay(10);
  }
  driveDirect = false;
  
  // Проверка таймаута поиска линии
  if (millis() - searchStartTime >= MAX_SEARCH_TIME) {
    lcdclear();
    lcdShow(0, 0, "Line not found");
    lcdShow(1, 0, "Timeout!");
    delay(1000);
  }
  
  // Сообщение о возврате на линию
  lcdclear();
  lcdShow(0, 0, "Back on track");
  delay(500);
}




int arrMove(int *starshiy)
{
  int arrChar = 64 * starshiy[6] + 32 * starshiy[5] + 16 * starshiy[4] + 8 * starshiy[3] + 4 * starshiy[2] + 2 * starshiy[1] + starshiy[0];
  // Serial.print(64 * starshiy[6]);
  // Serial.print(64 * starshiy[6]);
  // Serial.print(64 * starshiy[6]);
  // Serial.print(64 * starshiy[6]);
  // Serial.print(64 * starshiy[6]);
  // Serial.print(64 * starshiy[6]);
  /*Serial.println("starting arrMove");
  int arrChar;
  for (int i = 0; i < 7; i++)
  {
    arrChar = arrChar + binary[i] * (2 ^ (9 - i));
  }*/
  
  return arrChar;
}

// void cubeReverse(int n, const int arr[], int out[])
// {
//  //  void cubeReverse(int n, const int arr[], int out[], int start, int end)
//     //  // Проверка на корректность диапазона
//     // if (start < 0 || end >= CUBE_COUNT || start > end) {
//     //     // Если диапазон некорректен, копируем массив как есть
//     //     for (int i = 0; i < CUBE_COUNT; i++) {
//     //         out[i] = arr[i];
//     //     }
//     //     return;
//     // }

//     // // Копируем элементы до start
//     // for (int i = 0; i < start; i++) {
//     //     out[i] = arr[i];
//     // }

//     // // Зеркально копируем подмассив [start..end]
//     // Зеркалируем пары
//     for (int i = 0; i < length; i += 2) {
//         out[start + i] = arr[start + i + 1];
//         out[start + i + 1] = arr[start + i];
//     }

//     // // Копируем элементы после end
//     // for (int i = end + 1; i < CUBE_COUNT; i++) {
//     //     out[i] = arr[i];
//     // }

//     // Serial.println("Вход в функцию");
//     // Serial.println(n);
//      // Если reflectionPoint лежит в первой половине (включая середину),
//     // то зеркалим от 0 до reflectionPoint
//     int temp;
//     if (n <= 4) {
//         // 1) "Отзеркалим" диапазон [0 .. reflectionPoint]
//         //    Например, если reflectionPoint = 3, то:
//         //    b[0] = a[3], b[1] = a[2], b[2] = a[1], b[3] = a[0].
//         for (int i = 0; i <= n; i++) {
//             out[i] = arr[n * 2 - 1 - i];
//             //Serial.println("зеркаливание");
//         }
//         for (int i = n; i <= n * 2; i++) {
//             out[i] = arr[n - i + 3];
//             //Serial.println("зеркаливание");
//         }

//         // 2) Оставшуюся часть (с reflectionPoint+1 до конца массива) копируем как есть
//         for (int i = n * 2; i < CUBE_COUNT; i++) {
//              out[i] = arr[i];
//             // Serial.print("копирование элементов");
//             // Serial.println(i);
//         }
//     }
//     else {
//             // temp=arr[5];
//             // out[5] = arr[6];
//             // out[6] = temp;

//             // temp=arr[7];
//             // out[7] = arr[4];
//             // out[4] = temp;

//             // temp=arr[8];
//             // out[8] = arr[3];
//             // out[3] = temp;

//             // temp=arr[9];
//             // out[9] = arr[2];
//             // out[2] = temp;

//         // for (int i = n; i < CUBE_COUNT; i+2) {
//         //     temp=arr[i-1]; //сохраняю значение цвета записанное в исходном массиве в позиции черная линия -1 во временную переменную
//         //     out[i-1] = arr[i]; // записываю значение цвета из исходного массива из позиции где линия в результирующиц массив в предыдущую позицию
//         //     out[i] = temp;
//         //     //Serial.println("зеркаливание");
//         // }

//         // Если reflectionPoint не в первой половине,
//         // 1) "Отзеркалим" диапазон [reflectionPoint .. 9]
//         //    Например, если reflectionPoint = 7, то:
//         //    b[0] = a[3], b[1] = a[2], b[2] = a[1], b[3] = a[0].
//         for (int i = n; i <= 9; i++) {
//             out[i] = arr[n * 2 - i - 1];
//             //Serial.println("зеркаливание");
//         }
//         for (int i = n; i >= n - (CUBE_COUNT - n); i--) {
//             out[i] = arr[n * 2 - i + 1];
//             //Serial.println("зеркаливание");
//         }

//         // 2) Оставшуюся часть (с reflectionPoint+1 до конца массива) копируем как есть
//         for (int i = n - (CUBE_COUNT - n); i >= 0; i--) {
//             out[i] = arr[i];
//             //Serial.print("копирование элементов");
//             //Serial.println(i);
//         }

//     }
//     Serial.println("Измененный массив");
//     for (int i = 0; i < 10; i++)
//     {
//       Serial.print(out[i]);
//     }
//     Serial.println("Исходный массив");
//     for (int i = 0; i < 10; i++)
//     {
//       Serial.print(arr[i]);
//     }

// }


// void arrMove(int binary[], int n)
// {
//   Serial.println("starting arrMove");
//   int arrChar;
//   for (int i = 0; i < CUBE_COUNT; i++)
//   {
//     arrChar = arrChar + binary[i] * (2 ^ (9 - i));
//     Serial.println("making arrChar from binary to 10...");
//   }
//   Serial.println("making arrChar done");
//   Serial.println(arrChar);
//   if (arrChar % 2 == 0)
//   {
//     arrMoving(n);
//   }
// }

// void getCubeColors()
// {
//   //   int cube_cross = 0;
//   //   int cubeCrossCount;
//   int bloodCube;
//   for (int i = 0; i < CUBE_COUNT; i++)
//   {

//     clearAllEncoders();
//     while (getEncoder1() < CUBE_DISTANCE && getEncoder2() < CUBE_DISTANCE) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
//     {
//       pid(baseSpeed);
//     }
//     clearAllEncoders();
//     stop();

//     delay(500);
//     cubeColors[i] = colorObject();
//     if (cubeColors[i] == 0)
//     {
//       binary[CUBE_COUNT - i] = 1;
//     }
//     else
//     {
//       binary[CUBE_COUNT - i] = 0;
//     }
//     Serial.println("reading...");
//     delay(100);
//   }
//   for (int i = 0; i < CUBE_COUNT; i++)
//   {
//     cubeColorsREV[i] = cubeColors[i];
//   }
//   Serial.println("end of reading");
//   clearAllEncoders();
//   while (getEncoder1() < CUBE_DISTANCE && getEncoder2() < CUBE_DISTANCE) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
//   {
//     pid(baseSpeed);
//   }
//   clearAllEncoders();
//   stop();
//   Serial.println("reading bloodCube");
//   delay(500);
//   bloodCube = colorObject();
//   bloodCube = colorObject();
//   bloodCube = colorObject();
//   delay(100);

//   Serial.println(bloodCube);
//   Serial.print("[");
//   for (int i = 0; i < CUBE_COUNT; i++)
//   {
//     Serial.print(binary[i]); // Значение элемента
//     if (i < CUBE_COUNT - 1)
//       Serial.print(", ");
//   }
//   Serial.println("]");
//   arrMove(binary, bloodCube);
//   //  cubeReverse(cubeCrossCount, cubeColors, cubeColorsMirror);
//   Serial.println("Массив");
//   Serial.print("[");
//   for (int i = 0; i < CUBE_COUNT; i++)
//   {
//     Serial.print(cubeColors[i]); // Значение элемента
//     if (i < CUBE_COUNT - 1)
//       Serial.print(", ");
//   }
//   Serial.println("]");
// }

// void crossAction(int n)
// {
//   if (n == 1)
//   {
//     left();
//   }

//   else if (n == 2)
//   {
//     left();
//     left();
//   }

//   else if (n == 3)
//   {
//     right();
//   }

//   else
//   {
//     pid(baseSpeed);
//   }
// }

// void backCrossAction(int n)
// {

//   if (n == 1)
//   {
//     right();
//   }

//   else if (n == 2)
//   {
//     left();
//     left();
//   }

//   else if (n == 3)
//   {
//     left();
//   }

//   else
//   {
//     pid(baseSpeed);
//   }
// }

void driveToObjectOnBlack()
{
  while (getFDistance() > distanceToTakeBanka) // едем вперед на preg() пока расстояние до банки не будет меньше  distanceToTakeBanka
  {
    pid(baseSpeed);
  }
  stop(); // Ждем пока закончится импульс инерции
}

// едем назад пока не увидим черную линию
void driveBackToCross()
{
  while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
  startTime = millis();              // Считываем текущее время
  while (millis() - startTime < 200) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
  {
    stop();
  }
}

void fixPositionAfterTurn()
{
  // Выравниваем робота после поворота пока ошибка(разность показаний левого и правого ИК датчика) не будет меньше maxErrorTurnFix
  while (getIRError() < maxErrorTurnFix)
  {
    pid(0);
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
