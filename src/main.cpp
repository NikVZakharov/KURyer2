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

#include <arduino_encoder.h>

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

Encoder enc1;
Encoder enc2;

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

enc1.setup(6,7);
enc2.setup(8,9);

  for (int i = 2; i <= 5; i++)
    pinMode(i, OUTPUT);

  initServo();
#if DEBUG
  debug_init();
#else
  logInit();
#endif

  start();
  right();
  if (uzdF()<=20)
  {
    wallPosition=1;
  }
  else if (uzdF()>20 && uzdF()<30)
  {
    wallPosition=2;
  }
  else if (uzdF()>30 && uzdF()<40)
  {
    wallPosition=3;
  }
  else
  {
    wallPosition=4;
  }
  left();
  crossCount=0;
  
  
}

/*void ulica(int n)
{
  int N = (ABCD[n - 1]) - 101;
}*/

void cross(int n)
{
  
  if (analogRead(8) < 500 && analogRead(9) < 500)
  {
    int T = 0;
    go(baseSpeed,baseSpeed,baseDelay);
        T++;
    if (T == n)
    {
      go(0,0);
      while (1)
      {
        /* code */
      }
    }
  }
}

/*int l = 15;
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
}*/

void pidEnc(int V, int K, int n){
  int E=enc1.get()-enc2.get();
  int M1=V+E*K; M1=constrain(M1,-250,250);
  int M2=V-E*K; M2=constrain(M2,-250,250);
  go(M1,M2);
  if (enc1.get()>n||enc2.get()>n)
  {
    go(0,0); while(1);
    enc1.clear();
    enc2.clear();
  }
  
}

void leftEnc(int V, int K, int n){
  int E=abs(enc1.get())-abs(enc2.get());
  int M1=V+E*K; M1=constrain(M1,-250,250);
  int M2=V-E*K; M2=constrain(M2,-250,250);
  go(-M1,M2);
  if (abs(enc1.get())>n or abs(enc2.get())>n)
  {
    go(0,0); while(1);
    enc1.clear();
    enc2.clear();
  }
  
}

void pid(int V, float K){
  int E=analogRead(A0)-analogRead(A1);
  int M1=V+E*K; M1=constrain(M1,-250,250);
  int M2=V-E*K; M2=constrain(M2,-250,250);
  go(M1,M2);
 
  
}

void loop()
{
pidEnc(150, 10, 1350);

}
