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

int baseSpeed = 100; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
float KOEF_ERROR = 0.4;             // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int servoOpenPosition = 50;         // градус открытого серво
int servoClosePosition = 120;       // градус закытого серво
int baseDelay = 500;                // задержка между действиями
int crossCount = 0;                 // количество перекрестков
int crossDelay = 1000;              // то сколько проедет робот после того как датчики увидят перекресток
int timeToMoveBackWithBanka = 2000; // время, которое робот едет назад с банкой
int blackLimit = 600;               // все что ниже-черная линия
unsigned long startTime = 0;
int timeToCorrectTurn = 1000;
int distanceToTakeBanka = 5;   // расстояние на котром надо взять банку
int distanceToCheckBanka = 30; // расстояние на котром ищем банку

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
  pinMode(IR_SENSOR_L_PIN, INPUT); // датчик ИК - А0
  pinMode(IR_SENSOR_R_PIN, INPUT); // датчик ИК - А1
  pinMode(IR_SENSOR_M_PIN, INPUT); // датчик ИК - А2

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
  while (uzdF() > distanceToTakeBanka)
  {
    preg();
  }
  go(0, 0, baseDelay);
  closeServo();
  go(-baseSpeed, -baseSpeed, timeToMoveBackWithBanka);
  right();
  right();
  while (!isOnCross())
  {
    preg();
  }
  go(0, 0, baseDelay);

  // while (isOnBlack(IR_SENSOR_M_PIN))
  // {
  //   preg();
  // }
  // go(0, 0, baseDelay);
  // openServo();
  // go(-baseSpeed, -baseSpeed, baseDelay);
  // go(0, 0, baseDelay);
}

void moveBankaPut()
{
  while (!isOnBlack(IR_SENSOR_M_PIN)) // если датчик посередине на белом отпускаем банку
  {
    preg();
  }
  go(baseSpeed, baseSpeed, crossDelay);
  go(0, 0, baseDelay);
  openServo();
  //   go(-baseSpeed, -baseSpeed, crossDelay);
  go(0, 0, baseDelay);
}

void finish()
{
  go(baseSpeed, baseSpeed, crossDelay * 4);
  go(0, 0);
  while (true)
  {
  };
}

void loop()
{
#if !DEBUG
  //   consoleLog(baseDelay*2); //выводим информацию в консоль
#endif

  //   test();

  preg();

  if (isOnCross()) // наехал-ли робот на перекресток
  {
    crossCount++;
    if (crossCount == 4)
    {
      finish();
    }
    
    go(baseSpeed, baseSpeed, crossDelay);
    go(0, 0, baseDelay);
    right();

    if (uzdF() < distanceToCheckBanka) // есть ли банка справа
    {
      //  moveBankaTake();
      //  moveBankaPut();
    }
    else // иначе
    {
      right();
      right();
      if (uzdF() < distanceToCheckBanka) // есть ли банка слева
      {
        //   moveBankaTake();
        //  moveBankaPut();
        go(-baseSpeed, -baseSpeed, baseDelay * 1.5);
      }
    }
    right();// если нету банки слева или поставили банку
  }
}