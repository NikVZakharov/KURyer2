#define DEBUG 0

#if DEBUG
#include "avr8-stub.h"
#else

#endif

#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>

#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <log.h>

Servo servo;
const int UZF_TRIGGER_PIN = 7;
const int UZF_ECHO_PIN = 8;
// Устанавливаем номера пинов для датчиков линии
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;

int baseSpeed = 150;
int minIRL = 400, minIRR = 400, maxIRL = 600, maxIRR = 600;
int min1 = 400;
int max1 = 600;
int min2 = 400;
int max2 = 600;
float K = 0.4;
int N = 0;
int N1 = 0;
int VP = 90;
int servoOpenPosition = 50;
int servoClosePosition = 110;
int baseDelay = 500;

void setup()
{

  pinMode(2, OUTPUT); // напр. мотора лев.
  pinMode(3, OUTPUT); // скор. мотора лев.
  pinMode(4, OUTPUT); // напр. мотора прав.
  pinMode(5, OUTPUT); // скор. мотора прав.
  // pinMode(7, OUTPUT); // пинок дальномера uzdL - левого
  pinMode(UZF_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdF - фронтального
  // pinMode(8, INPUT);  // эхо-прием дальномера uzdL - левого
  pinMode(UZF_ECHO_PIN, INPUT); // эхо-прием дальномера uzdF - фронтального
  pinMode(A0, INPUT);           // датчик ИК - А0
  pinMode(A1, INPUT);           // датчик ИК - А1
  pinMode(A2, INPUT);           // датчик ИК - А2
  servo.attach(13);
  servo.write(servoOpenPosition);
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
  while (!isOnCross)
  {
    preg();
  }
  go(0, 0);
  openServo();
}

void closeServo()
{
  for (int i = servoOpenPosition; i < servoClosePosition; i++)
  {
    servo.write(i);
    delay(20);
  }
}

void openServo()
{
  for (int i = servoClosePosition; i < servoOpenPosition; i++)
  {
    servo.write(i);
    delay(20);
  }
}

void loop()
{
#if !DEBUG
  //   consoleLog(baseDelay*2); //выводим информацию в консоль
#endif

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
        preg();
      }
    }

    while (1)
    {
      /* code */
    }
  }
}