#include <Wire.h>
#include <Arduino.h>
#include <NewPing.h>

#include <drive.h>
#include <middleware.h>
#include <uzd.h>
#include <header.h>
#include <servoMotor.h>
#include <banka.h>
#include <LCD.h>
#include <encoder.h>

#include <log.h>
#include <test.h>

// ################## Константы ###################
// ################### Настройка пинов #############

const int MOTOR_L_DIRECTION_PIN = 2;
const int MOTOR_L_SPEED_PIN = 3;
const int MOTOR_R_DIRECTION_PIN = 4;
const int MOTOR_R_SPEED_PIN = 5;
const int UZS_TRIGGER_PIN = 6;
const int UZS_ECHO_PIN = 7;
const int UZF_TRIGGER_PIN = 12;
const int UZF_ECHO_PIN = 13;
const int MOTOR_R_ENCODER_PIN1 = 8;
const int MOTOR_R_ENCODER_PIN2 = 9;
const int MOTOR_L_ENCODER_PIN1 = 10;
const int MOTOR_L_ENCODER_PIN2 = 11;
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int IR_SENSOR_M_PIN = A7;
const int SERVO_PIN = A3;
// ################### Настройки программы ############

const bool IS_IR_SENSORS_REVERS = false;

// const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
// const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;

// ############## Переменные ####################

// Базовые параметры
int baseSpeed = 100; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
int baseDelay = 500;         // задержка между действиями
unsigned long startTime = 0; // Время начала таймера
int finishDelay = 600;       // задержка при финишировании
int timeToShowLED = 10;      // время вывода информации на lcd дисплей
int testTime = 1000;         // время для тестирования функции
int blackLimit = 700; //   все что ниже-черная линия 

// UZD
float pastUZDFValue; // предыдущее значение переднего датчика 
float pastUZDSValue; // предыдущее значение бокового датчика

// П регулятор
float koef_preg_p = 0.5;  // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int gainCoeff = 300;      // Коэффициент усиления П регулятора при выравнивании после поворота

int walldistance = 8;     // расстояние до стены
bool maze = true;         // едем в лабиринте

int Ei = 0;


float Kp; // пропорциональный
float Ki; // интегральный
float Kd; // диференциальный
int Ep;
int wallDistance = 15; // расстояние до стены

// Сервопривод
int servoOpenPosition = 30;   // градус открытого серво
int servoClosePosition = 150; // градус закытого серво

// Перекрестки
int crossCount = 0;        // количество перекрестков
int crossDelay = 300;      // то сколько проедет робот после того как датчики увидят перекресток

// Работа с банкой
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
unsigned long timeToMoveBanka = 200; // Время в течении которого выравниваем машину после поворота
unsigned long timeToMoveBanka2 = 2000;
int distanceToTakeBanka = 6;   // расстояние на котром надо взять банку
int distanceToCheckBanka = 20; // расстояние на котром ищем банку
bool haveBanka = false;        // Флаг обнаружения банки -есть или нет банки на по направлению движения
int obezdDelay = 3000;         // задержка при объезде банки
int otezdDelay = 500;
int banka = 0;

// Поворот
int maxErrorTurnFix = 10; // Макисмальная ошибка до которой идет выравнивание после поворота
int povorotDelay = 1100;  // задержка при повороте на 90 градусов
int povorotDelay2 = 200;

// Движение вне линии
int obezdObjectDelay = 1000;
int distanceToCheckObject = 40;

// Финиш
int FINISH_CROSS_COUNT = 21; // номер перекрестка на которам надо финишировать
int finishcount = 0;         // увеличиваем crosscount при случае если надо финишировать на определенном перекрестке

int whiteEdgeLimit = 550;
int blackEdgeLimit = 150;
int wallPosition = 1;

bool coordinatesSet = false;
int maxn = 0;
bool coordset = false;

void setup()
{

  // put your setup code here, to run once: v

  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT); // напр. мотора лев.
  pinMode(MOTOR_L_SPEED_PIN, OUTPUT);     // скор. мотора лев.
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT); // напр. мотора прав.
  pinMode(MOTOR_R_SPEED_PIN, OUTPUT);     // скор. мотора прав.
  pinMode(UZF_TRIGGER_PIN, OUTPUT);       // пинок дальномера uzdF - фронтального
  pinMode(UZF_ECHO_PIN, INPUT);           // эхо-прием дальномера uzdF - фронтального
  pinMode(UZS_TRIGGER_PIN, OUTPUT);       // пинок дальномера uzdS - бокового
  pinMode(UZS_ECHO_PIN, INPUT);           // эхо-прием дальномера uzdS - бокового
  pinMode(IR_SENSOR_L_PIN, INPUT);        // пин левого датчика ИК
  pinMode(IR_SENSOR_R_PIN, INPUT);        // пин правого датчика ИК
  pinMode(IR_SENSOR_M_PIN, INPUT);        // пин правого датчика ИК
  pinMode(SERVO_PIN, OUTPUT);             // серво привод

  initLCD();
  initENC();
  initServo();
  logInit();
  // start();
}

void loop()
{
  // ##### Тесты Начало ######
  //go(baseSpeed, -baseSpeed);
   //test();
  // consoleLog();
  // ##### Тесты Конец ######1488


// pidEncSomeTime(4000);
// stop();
// delay(1000);
turnEnc(100,90);



}
