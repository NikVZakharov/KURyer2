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

#define DEBUG 0

#if DEBUG
#include "avr8-stub.h"
#else

#endif

const int UZF_TRIGGER_PIN = 8;
const int UZF_ECHO_PIN = 9;
// Устанавливаем номера пинов для датчиков линии
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int IR_SENSOR_M_PIN = A2;
const int MOTOR_L_DIRECTION_PIN = 4;
const int MOTOR_L_SPEED_PIN = 5;
const int MOTOR_R_DIRECTION_PIN =2;
const int MOTOR_R_SPEED_PIN = 3;
const int SERVO_PIN = 13;
const int MOTOR_L_ENCODER_PIN1=6 ;
const int MOTOR_L_ENCODER_PIN2=7 ;
const int MOTOR_R_ENCODER_PIN1=8 ;
const int MOTOR_R_ENCODER_PIN2=9; 

const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;

int baseSpeed = 150; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
float koef_preg_p = 0.4;              // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int servoOpenPosition = 60;          // градус открытого серво
int servoClosePosition = 130;        // градус закытого серво
int baseDelay = 1000;                // задержка между действиями
int crossCount = 0;                  // количество перекрестков
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
unsigned long startTime = 0;         // Время начала таймера
unsigned long timeToMoveBanka = 900; // Время в течении которого выравниваем машину после поворота

int distanceToTakeBanka = 6;   // расстояние на котром надо взять банку
int distanceToCheckBanka = 20; // расстояние на котром ищем банку

bool haveBanka = false;   // Флаг обнаружения банки -есть или нет банки на по направлению движения
int gainCoeff = 300;      // Коэффициент усиления П регулятора при выравнивании после поворота
int maxErrorTurnFix = 10; // Макисмальная ошибка до которой идет выравнивание после поворота
int obezdDelay = 1500;    // задержка при объезде банки

int finishDelay = 600; // задержка при финишировании

int povorotDelay = 1000; // задержка при повороте на 90 градусов
int obezdObjectDelay = 2000;
int distanceToCheckObject = 30;

int crossDelay = 600; // то сколько проедет робот после того как датчики увидят перекресток
int whiteEdgeLimit = 550;
int blackEdgeLimit = 150;
int blackLimitPreg = 900;  // все что ниже-черная линия для Прегулятора
int blackLimitCross = 400; //   все что ниже-черная линия для определения перекрестка

int wallPosition = 1;
int FINISH_CROSS_COUNT = 6;
int finishcount = 0;
bool coordinatesSet = false;

int maxn = 0;
bool coordset = false;

int timeToShowLED= 50; // время вывода информации на lcd дисплей

void setup()
{

  // put your setup code here, to run once: v
  for (int i = 2; i <= 5; i++)
    pinMode(i, OUTPUT);

  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT); // напр. мотора лев.
  pinMode(MOTOR_L_SPEED_PIN, OUTPUT);     // скор. мотора лев.
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT); // напр. мотора прав.
  pinMode(MOTOR_R_SPEED_PIN, OUTPUT);     // скор. мотора прав.
  //  pinMode(7, OUTPUT); // пинок дальномера uzdL - левого
  // pinMode(UZF_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdF - фронтального
  // // pinMode(8, INPUT);  // эхо-прием дальномера uzdL - левого
  // pinMode(UZF_ECHO_PIN, INPUT);    // эхо-прием дальномера uzdF - фронтального
  pinMode(IR_SENSOR_L_PIN, INPUT); // пин датчика ИК - А0
  pinMode(IR_SENSOR_R_PIN, INPUT); // пин датчика ИК - А1
  // pinMode(IR_SENSOR_M_PIN, INPUT); // пин датчика ИК - А2

  initLCD();
  initENC();
  initServo();
#if DEBUG
  debug_init();
#else
  logInit();
#endif

  // start();
}

/*void ulica(int n)
{

//go(-baseSpeed,baseSpeed);
 //preg(baseSpeed);
  encpid(baseSpeed,koef_preg_p);
  
// Serial.println(IR_SENSOR_L_PIN);
// Serial.println(" ");
// Serial.println(IR_SENSOR_R_PIN);

//   if (isOnCross())
//   {
//     go(baseSpeed,baseSpeed,700);go(0,0,500);
//     crossCount++;

//     if (crossCount==3)
//     {
//       right();
//     }
//     if (crossCount==4)
//     {
//       left();
//     }
// if (crossCount==5)
//     {
//       right();
//     }
// if (crossCount==6)
//     {
//       left();
//     }
// if (crossCount==7)
//     {
//       left;
//       left;
//     }
//   if (crossCount==8)
//     {
//       right();
//     }
//    if (crossCount==9)
//     {
//       left();
//     }
//    if (crossCount==11)
//     {
//       right();
//     }
//  if (crossCount==12)
//     {
//       right();
//     }
//    if (crossCount==14)
//     {
//       left();
//       left();
//     }
//    if (crossCount==15)
//     {
//       right();
//     }
    

//     if(crossCount==16)
//      {
//       go(0,0);while(1);
//     }
 // }
  
}
