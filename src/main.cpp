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

//#include <LiquidCrystal.h>
//LiquidCrystal LCD(6,7,8,9,10,11);

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
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
unsigned long startTime = 0;         // Время начала таймера
unsigned long timeToMoveBanka = 900; // Время в течении которого выравниваем машину после поворота

int distanceToTakeBanka = 6;         // расстояние на котром надо взять банку
int distanceToCheckBanka = 20;       // расстояние на котром ищем банку

bool haveBanka = false;              // Флаг обнаружения банки -есть или нет банки на по направлению движения
int gainCoeff = 300;                 // Коэффициент усиления П регулятора при выравнивании после поворота
int maxErrorTurnFix = 10;            // Макисмальная ошибка до которой идет выравнивание после поворота
int obezdDelay = 1500;               // задержка при объезде банки

int finishDelay = 600;              // задержка при финишировании

int povorotDelay = 1000;             // задержка при повороте на 90 градусов
int obezdObjectDelay = 2000;
int distanceToCheckObject = 30;

int crossDelay = 600;                // то сколько проедет робот после того как датчики увидят перекресток
int whiteEdgeLimit = 550;
int blackEdgeLimit = 150;
int blackLimitPreg = 500;            // все что ниже-черная линия для Прегулятора
int blackLimitCross = 400;           //   все что ниже-черная линия для определения перекрестка

int wallPosition=1;
int FINISH_CROSS_COUNT = 6;
int finishcount=0;



char abcd=' ';
char ABCD[6];
bool flag=0;


void setup()
{
for(int i=2; i<=5; i++)pinMode(i,OUTPUT);
  for(int i=12; i<=16; i++)pinMode(i,INPUT);
  //LCD.begin(16,2);
  //LCD.print("jndkdjnwkjcn");

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

void loop(){

preg(baseSpeed);
doezd();


if (isOnCross())
{
  /**/
  while (crossCount<crossCount+ABCD[0]-'A')
  {
    preg(baseSpeed);
  }
  if (ABCD[1]-'3'>0)
  {
    left();
    while (crossCount<crossCount+abs(ABCD[1]-'3'))
  {
    preg(baseSpeed);
  }
  }
  else
  {
      right();
    while (crossCount<crossCount+abs(ABCD[1]-'3'))
  {
    preg(baseSpeed);
  }
  }
  
  left();
  left();
    if (ABCD[1]-'3'>0)
  {
    while (crossCount<crossCount+abs(ABCD[1]-'3'))
  {
    preg(baseSpeed);
  }

  right();
  }

  else
  {
      
    while (crossCount<crossCount+abs(ABCD[1]-'3'))
  {
    preg(baseSpeed);
  }
  left();
  }

  while (crossCount<crossCount+ABCD[0]-'A')
  {
    preg(baseSpeed);
  }
  
 /**/ 

/**/
  while (crossCount<crossCount+ABCD[2]-'A')
  {
    preg(baseSpeed);
  }
  if (ABCD[3]-'3'>0)
  {
    left();
    while (crossCount<crossCount+abs(ABCD[3]-'3'))
  {
    preg(baseSpeed);
  }
  }
  else
  {
      right();
    while (crossCount<crossCount+abs(ABCD[3]-'3'))
  {
    preg(baseSpeed);
  }
  }
  
  left();
  left();
    if (ABCD[3]-'3'>0)
  {
    while (crossCount<crossCount+abs(ABCD[3]-'3'))
  {
    preg(baseSpeed);
  }

  right();
  }

  else
  {
      
    while (crossCount<crossCount+abs(ABCD[3]-'3'))
  {
    preg(baseSpeed);
  }
  left();
  }

  while (crossCount<crossCount+ABCD[2]-'A')
  {
    preg(baseSpeed);
  }
  
 /**/ 

/**/
  while (crossCount<crossCount+ABCD[4]-'A')
  {
    preg(baseSpeed);
  }
  if (ABCD[5]-'3'>0)
  {
    left();
    while (crossCount<crossCount+abs(ABCD[5]-'3'))
  {
    preg(baseSpeed);
  }
  }
  else
  {
      right();
    while (crossCount<crossCount+abs(ABCD[5]-'3'))
  {
    preg(baseSpeed);
  }
  }
  
  left();
  left();
    if (ABCD[5]-'3'>0)
  {
    while (crossCount<crossCount+abs(ABCD[5]-'3'))
  {
    preg(baseSpeed);
  }

  right();
  }

  else
  {
      
    while (crossCount<crossCount+abs(ABCD[5]-'3'))
  {
    preg(baseSpeed);
  }
  left();
  }

  while (crossCount<crossCount+ABCD[4]-'A')
  {
    preg(baseSpeed);
  }
  
 /**/ 


}

  



}




