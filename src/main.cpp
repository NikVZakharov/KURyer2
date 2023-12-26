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
const int FINISH_CROSS_COUNT = 12;
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
int crossDelay = 500;                // то сколько проедет робот после того как датчики увидят перекресток
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
int blackLimit = 500;                // все что ниже-черная линия
unsigned long startTime = 0;         // Время начала таймера
unsigned long timeToMoveBanka = 900; // Время в течении которого выравниваем машину после поворота
int distanceToTakeBanka = 5;         // расстояние на котром надо взять банку
int distanceToCheckBanka = 30;       // расстояние на котром ищем банку
bool haveBanka = false;              // Флаг обнаружения банки -есть или нет банки на по направлению движения
int gainCoeff = 50;                  // Коэффициент усиления П регулятора при выравнивании после поворота
int maxErrorTurnFix = 10;            // Макисмальная ошибка до которой идет выравнивание после поворота
int obezdDelay = 1500;               // задержка при объезде банки
int finishDelay = 2000;              // задержка при финишировании
int povorotDelay = 1000;             // задержка при повороте на 90 градусов

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
  for (int i = 2; i <= 5; i++)
    pinMode(i, OUTPUT);

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
  doezd();

  // while (!isOnCross()) // Едем вперед пока не доедем до перекрестка
  // {
  //   preg(baseSpeed);
  // }
  // go(0, 0, baseDelay); // Ждем пока закончится импульс инерции
}

void moveBankaPut()
{
  // while (isOnBlack(IR_SENSOR_M_PIN))
  // {
  //   preg(baseSpeed);
  // }
  pregSomeTime(timeToMoveBanka);

  go(0, 0, baseDelay);
  openServo();
  while (!isOnCross())
  {
    go(-baseSpeed, -baseSpeed);
  }
  doezd();
}

void MoveBanka90grad()
{

  moveBankaTake();
  right();
  right();
  right();
  moveBankaPut();
  left();
}

void moveBankaNextCross()
{
  go(baseSpeed, -baseSpeed, povorotDelay);
  while (distanceToTakeBanka > 10)
  {
    go(baseSpeed, baseSpeed);
  }
  moveBankaTake();
  doezd();
  left();
}

int l = 15;
int Kp = 20, Ki = 0, Kd = 0;
float I = 0;
int S=0;
int uzd(int tr, int echo) {
  pinMode(tr,OUTPUT); pinMode(echo,INPUT);
  digitalWrite(7, 0);
  delayMicroseconds(2);
  digitalWrite(7, 1);
  delayMicroseconds(10);
  digitalWrite(7, 0);
  S=0.6*S+0.4*(0.01723 * pulseIn(8, 1));
  return S;
}
void loop()
{

  // test();

  int E = uzd(9,10) - l;
  int D = E - I;
  int U = Kp * E + Ki * I + Kd * D;
  int M1 = baseSpeed + U;
  M1 = constrain(M1, -255, 255);
  int M2 = baseSpeed - U;
  M2 = constrain(M2, -255, 255);
  go(M1, M2);
  I=0.95*I+E;
}
