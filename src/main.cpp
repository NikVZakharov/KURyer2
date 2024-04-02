#include <Wire.h>
#include <Arduino.h>
#include <NewPing.h>

#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <banka.h>
#include <LCD.h>
#include <encoder.h>

#include <log.h>
#include <test.h>

// ################## Константы ###################
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int SERVO_PIN = A3;
const int MOTOR_R_DIRECTION_PIN = 4;
const int MOTOR_R_SPEED_PIN = 5;
const int MOTOR_L_DIRECTION_PIN = 2;
const int MOTOR_L_SPEED_PIN = 3;
const int UZF_ECHO_PIN = 8;
const int UZF_TRIGGER_PIN = 9;
const int MOTOR_L_ENCODER_PIN1 = 10;
const int MOTOR_L_ENCODER_PIN2 = 11;
const int MOTOR_R_ENCODER_PIN1 = 12;
const int MOTOR_R_ENCODER_PIN2 = 13;

// const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
// const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;

// ############## Переменные ####################

// Базовые параметры
int baseSpeed = 250; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
int baseDelay = 100;        // задержка между действиями
unsigned long startTime = 0; // Время начала таймера
int finishDelay = 600;       // задержка при финишировании
int timeToShowLED = 50;      // время вывода информации на lcd дисплей

// П регулятор
float koef_preg_p = 0.4;  // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int gainCoeff = 300;      // Коэффициент усиления П регулятора при выравнивании после поворота
int blackLimitPreg = 750; // все что ниже-черная линия для Прегулятора

// Сервопривод
int servoOpenPosition = 60;   // градус открытого серво
int servoClosePosition = 140; // градус закытого серво

// Перекрестки
int crossCount = 0;        // количество перекрестков
int crossDelay = 300;     // то сколько проедет робот после того как датчики увидят перекресток
int blackLimitCross = 750; //   все что ниже-черная линия для определения перекрестка

// Работа с банкой
int timeToMoveBackWithBanka = 1000;   // время, которое робот едет назад с банкой
unsigned long timeToMoveBanka = 2000; // Время в течении которого выравниваем машину после поворота
int distanceToTakeBanka = 6;          // расстояние на котром надо взять банку
int distanceToCheckBanka = 20;        // расстояние на котром ищем банку
bool haveBanka = false;               // Флаг обнаружения банки -есть или нет банки на по направлению движения
int obezdDelay = 1500;                // задержка при объезде банки

// Поворот
int maxErrorTurnFix = 10; // Макисмальная ошибка до которой идет выравнивание после поворота
int povorotDelay = 400;  // задержка при повороте на 90 градусов
int povorotDelay2=200;

// Движение вне линии
int obezdObjectDelay = 2000;
int distanceToCheckObject = 30;

int whiteEdgeLimit = 550;
int blackEdgeLimit = 150;
int wallPosition = 1;
int FINISH_CROSS_COUNT = 9;
int finishcount = 0;
bool coordinatesSet = false;
int maxn = 0;
bool coordset = false;

void setup()
{

  // put your setup code here, to run once: v
  for (int i = 2; i <= 5; i++)
    pinMode(i, OUTPUT);

  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT); // напр. мотора лев.
  pinMode(MOTOR_L_SPEED_PIN, OUTPUT);     // скор. мотора лев.
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT); // напр. мотора прав.
  pinMode(MOTOR_R_SPEED_PIN, OUTPUT);     // скор. мотора прав.
  pinMode(UZF_TRIGGER_PIN, OUTPUT);       // пинок дальномера uzdF - фронтального
  pinMode(UZF_ECHO_PIN, INPUT);           // эхо-прием дальномера uzdF - фронтального
  pinMode(IR_SENSOR_L_PIN, INPUT);        // пин датчика ИК - А0
  pinMode(IR_SENSOR_R_PIN, INPUT);        // пин датчика ИК - А1
  pinMode(SERVO_PIN, OUTPUT);             // серво привод

  initLCD();
  initENC();
  initServo();

  start();
}

void loop()
{
  // ##### Тесты Начало ######

  // test();
  // go(baseSpeed,baseSpeed, baseDelay*2);
  // go(0,0,baseDelay*2);
  // preg(baseSpeed);
  // encpid(baseSpeed,koef_preg_p);

  // ##### Тесты Конец ######

  preg(baseSpeed);
  if (isOnCross())
  {
    doezd();
    crossCount++;
    finish();

    if (crossCount == 1)
    {
      left();
    }
    if (crossCount == 2)
    {
      right();
    }
    if (crossCount==3)
        {
          right();
        }
    if (crossCount == 4)
    {
      left();
    }
    // if (crossCount == 5)
    // {
    //   left();
    //   left();
    // }
    if (crossCount==6)
      {
        left();
      }
    if (crossCount == 7)
    {
      right();
    }
    if (crossCount == 8)
    {
      left();
    }
    

  }
 }
