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
const int FINISH_CROSS_COUNT = 4;

int baseSpeed = 150; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
float KOEF_ERROR = 0.4;             // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int servoOpenPosition = 60;         // градус открытого серво
int servoClosePosition = 130;       // градус закытого серво
int baseDelay = 1000;               // задержка между действиями
int crossCount = 0;                 // количество перекрестков
int crossDelay = 500;               // то сколько проедет робот после того как датчики увидят перекресток
int timeToMoveBackWithBanka = 1000; // время, которое робот едет назад с банкой
int blackLimit = 500;               // все что ниже-черная линия
unsigned long startTime = 0;        // Время начала таймера
float timeToCorrectTurn = 1000;     // Время в течении которого выравниваем машину после поворота
int distanceToTakeBanka = 5;        // расстояние на котром надо взять банку
int distanceToCheckBanka = 30;      // расстояние на котром ищем банку
bool haveBanka = false;             // Флаг обнаружения банки -есть или нет банки на по направлению движения
int gainCoeff = 100;                // Коэффициент усиления П регулятора при выравнивании после поворота
int maxErrorTurnFix = 10;           // Макисмальная ошибка до которой идет выравнивание после поворота

void start()
{
  // Едем вперед пока не увидим поперечную черную линию
  while (!isOnCross())
  {
    go(baseSpeed, baseSpeed);
  }
  go(baseSpeed, baseSpeed, 300); // проезжаем поперечную черную линию пока черная линия трассы не окажется между датчиками
}

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

void moveBankaTake()
{
  while (uzdF() > distanceToTakeBanka) // едем вперед на preg() пока расстояние до банки не будет меньше  distanceToTakeBanka
  {
    preg(baseSpeed);
    // go(baseSpeed, baseSpeed);
  }
  go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
  closeServo();        // закрываем сервопривод
  while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
  go(baseSpeed, baseSpeed, crossDelay);
  go(0, 0, baseDelay);
  right();
  right();
  go(0, 0, baseDelay);
  // while (!isOnCross()) // Едем вперед пока не доедем до перекрестка
  // {
  //   preg();
  // }
  // go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
}

void moveBankaPut()
{
  while (isOnBlack(IR_SENSOR_M_PIN))
  {
    preg(baseSpeed);
  }
  go(0, 0, baseDelay);
  openServo();
  while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
  go(baseSpeed, baseSpeed, baseDelay / 2);
  go(0, 0, baseDelay);
}

void finish()
{
  if (crossCount == FINISH_CROSS_COUNT)
  {
    go(baseSpeed, baseSpeed, crossDelay * 3);
    go(0, 0);
    while (true)
    {
    };
  }
}

void loop()
{
   preg(baseSpeed);
   while (uzdF()>distanceToCheckBanka/2)
   {
    preg(baseSpeed);
   }
   go(0,0,baseDelay);
   go(-baseSpeed,baseSpeed,baseDelay);
   go(0,0,baseDelay);
   go(baseSpeed,baseSpeed,baseDelay*1.5);
   go(baseSpeed,-baseSpeed,baseDelay);
   go(baseSpeed,baseSpeed,baseDelay*3);
   go(baseSpeed,-baseSpeed,baseDelay);
   while (IR_SENSOR_R_PIN>blackLimit)
   {
    go(baseSpeed,baseSpeed);
   }
   preg(baseSpeed);
   
// test();

 /* preg(baseSpeed);
  if (isOnCross())
  {
    crossCount++;
    finish();
    go(baseSpeed, baseSpeed, crossDelay);
    go(0, 0, baseDelay);
    right();

    if (uzdF() < distanceToCheckBanka)
    {
      moveBankaTake();
      moveBankaPut();
      right(false);
    }
    else
    {
      right();
      right();
      if (uzdF() < distanceToCheckBanka)
      {
        moveBankaTake();
        moveBankaPut();
        left();
      }
      else
      {
        right();
      }
    }
    
  }*/
}

// crossCount=crossCount+1;
//   Serial.println(crossCount);
//   delay(500);



 