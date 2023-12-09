#define DEBUG 0

#if DEBUG
#include "avr8-stub.h"
#else

#endif

#include <Arduino.h>
#include <NewPing.h>

#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <banka.h>


#include <log.h>
#include <test.h>

const int UZF_TRIGGER_PIN = 7;
const int UZF_ECHO_PIN = 8;
// Устанавливаем номера пинов для датчиков линии
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int IR_SENSOR_M_PIN = A2;
const int MOTOR_L_DIRECTION_PIN = 2;
const int MOTOR_L_SPEED_PIN = 3;
const int MOTOR_R_DIRECTION_PIN = 4;
const int MOTOR_R_SPEED_PIN = 5;
const int SERVO_PIN = 13;
const int FINISH_CROSS_COUNT = 9;
const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;

int baseSpeed = 200; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
float KOEF_ERROR = 0.4;              // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int servoOpenPosition = 60;          // градус открытого серво
int servoClosePosition = 130;        // градус закытого серво
int baseDelay = 1000;                // задержка между действиями
int crossCount = 0;                  // количество перекрестков
int crossDelay = 700;                // то сколько проедет робот после того как датчики увидят перекресток
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
int blackLimit = 400;                // все что ниже-черная линия
unsigned long startTime = 0;         // Время начала таймера
unsigned long timeToMoveBanka = 900; // Время в течении которого выравниваем машину после поворота
int distanceToTakeBanka = 6;         // расстояние на котром надо взять банку
int distanceToCheckBanka = 30;       // расстояние на котром ищем банку
bool haveBanka = false;              // Флаг обнаружения банки -есть или нет банки на по направлению движения
int gainCoeff = 100;                 // Коэффициент усиления П регулятора при выравнивании после поворота
int maxErrorTurnFix = 10;            // Макисмальная ошибка до которой идет выравнивание после поворота
int obezdDelay = 1500;               // задержка при объезде банки
int finishDelay = 2000;              // задержка при финишировании
int povorotDelay = 1000;             // задержка при повороте на 90 градусов
int obezdObjectDelay = 2000;
int distanceToCheckObject = 20;

void setup()
{
  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT); // напр. мотора лев.
  pinMode(MOTOR_L_SPEED_PIN, OUTPUT);     // скор. мотора лев.
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT); // напр. мотора прав.
  pinMode(MOTOR_R_SPEED_PIN, OUTPUT);     // скор. мотора прав.
  // pinMode(7, OUTPUT); // пинок дальномера uzdL - левого
  pinMode(UZF_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdF - фронтального
  // pinMode(8, INPUT);  // эхо-прием дальномера uzdL - левого
  pinMode(UZF_ECHO_PIN, INPUT);    // эхо-прием дальномера uzdF - фронтального
  pinMode(IR_SENSOR_L_PIN, INPUT); // пин датчика ИК - А0
  pinMode(IR_SENSOR_R_PIN, INPUT); // пин датчика ИК - А1
  pinMode(IR_SENSOR_M_PIN, INPUT); // пин датчика ИК - А2

  initServo();
#if DEBUG
  debug_init();
#else
  logInit();
#endif

  start();
}

void loop()
{

  preg(baseSpeed); // едем по preg с базовой скоростью

   //test();

   obezdObject(); // проверяем нужно ли нам объехать банку

  // если мы попали на резкий поворот то мы проверяем находятся ли правый и центральный датчик на черном и поворачиваем
  /*if (IR_SENSOR_R_PIN < blackLimit && IR_SENSOR_M_PIN < blackLimit && IR_SENSOR_L_PIN > blackLimit)
  {
    Serial.print("hello");
    go(0, 0, 500);
    rightWith();
    Serial.println("end");
    
  }*/
  

  if (isOnCross())
  {

    crossCount++;
    finish();
    doezd();
    

    if (crossCount == 1)
    {
      right();
    }
    else if (crossCount == 6)
    {
      left();
      while (!isOnCross())
      {
        preg(baseSpeed);
      }
      doezd();
      driveBackToCross();
      doezd();
      left();
      left();
    }
    else if (crossCount==7)
    {
      doezd();
    }

    else if (crossCount == 8)
    {
      left();
      moveToTakeObjectOnBlack();
      right();
      right();
      moveToPutObjectOnBlack();
      left();
    }
    else
    {
      perekrestok();
    }
    
  }
}
