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
const int DVIG_L_MOTOR_PIN=2;
const int SPEED_DVIG_L_MOTOR_PIN=3;
const int DVIG_R_MOTOR_PIN=4;
const int SPEED_DVIG_R_MOTOR_PIN=5;
const int SERVO_PIN=13;

int baseSpeed = 150;
int minIRL = 400, minIRR = 400, maxIRL = 600, maxIRR = 600;
float KOEF_ERROR = 0.4;
int servoOpenPosition = 50;
int servoClosePosition = 110;
int baseDelay = 500;

void setup()
{

  pinMode(DVIG_L_MOTOR_PIN, OUTPUT); // напр. мотора лев.
  pinMode(SPEED_DVIG_L_MOTOR_PIN, OUTPUT); // скор. мотора лев.
  pinMode(DVIG_R_MOTOR_PIN, OUTPUT); // напр. мотора прав.
  pinMode(SPEED_DVIG_R_MOTOR_PIN, OUTPUT); // скор. мотора прав.
  // pinMode(7, OUTPUT); // пинок дальномера uzdL - левого
  pinMode(UZF_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdF - фронтального
  // pinMode(8, INPUT);  // эхо-прием дальномера uzdL - левого
  pinMode(UZF_ECHO_PIN, INPUT); // эхо-прием дальномера uzdF - фронтального
  pinMode(IR_SENSOR_L_PIN, INPUT);           // датчик ИК - А0
  pinMode(IR_SENSOR_R_PIN, INPUT);           // датчик ИК - А1
 // pinMode(A2, INPUT);           // датчик ИК - А2
initServo();
#if DEBUG
  debug_init();
#else
  logInit();
#endif

  // Едем вперед пока не увидим поперечную черную линию
  while (!isOnCross())
  {
    go(baseSpeed, baseSpeed);
  }
  go(baseSpeed, baseSpeed, 300); // проезжаем поперечную черную линию пока черная линия трассы не окажется между датчиками
}

void moveBanka()
{

  while (uzdF() > 7)
  {
    preg();
  }
  go(0, 0, baseDelay);
  closeServo();
  delay(baseDelay);
  go(-baseSpeed, -baseSpeed, baseDelay);
  right();
  right();
  while (!isOnCross())
  {
    preg();
  }
  go(0, 0);
  openServo();
}



void loop()
{
#if !DEBUG
  //   consoleLog(baseDelay*2); //выводим информацию в консоль
#endif

// test();

  preg();

  if (isOnCross())
  {

    go(baseSpeed, baseSpeed, baseDelay);
    go(0, 0, baseDelay);
    right();
    delay(baseDelay);
    if (uzdF() > 20)
    {
      moveBanka();
    }
    else
    {
      right();
      right();
      if (uzdF() > 20)
      {
        moveBanka();
        go(-baseSpeed, -baseSpeed, baseDelay);
        left();
       
      }
    }

    while (1)
    {
      /* code */
    }
  }
}