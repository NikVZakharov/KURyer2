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
const int UZF_TRIGGER_PIN = A3;
const int UZF_ECHO_PIN = A2;
const int MOTOR_L_ENCODER_PIN1 = 8;
const int MOTOR_L_ENCODER_PIN2 = 9;
const int MOTOR_R_ENCODER_PIN1 = 10;
const int MOTOR_R_ENCODER_PIN2 = 11;
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int IR_SENSOR_M_PIN = A7;
const int SERVO_PIN = A6;

// ################### Настройки программы ############

const bool IS_IR_SENSORS_REVERS = false;

// const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
// const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;
const int meanArraySize=5; //размер массива усреднения
const float MAX_INTEGRAL = 100; // Максимальное значение интегральной составляющей для защиты от wind-up
const int CROSS_WIDTH=200; //ширина линии перекрестка
const int START_WIDTH=1000;//длина старта


// ############## Переменные ####################

// Базовые параметры
int baseSpeed = 200; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
int baseDelay = 500;         // задержка между действиями
unsigned long startTime = 0; // Время начала таймера
int finishDelay = 600;       // задержка при финишировании
int timeToShowLED = 10;      // время вывода информации на lcd дисплей
int testTime = 1000;         // время для тестирования функции
int blackLimit = 600; //   все что ниже-черная линия 

// UZD

float pastUZDFValue[meanArraySize]  = {0}; // предыдущее значение переднего датчика 
float pastUZDSValue[meanArraySize]  = {0}; // предыдущее значение бокового датчика

// П регулятор
float koef_preg_p = 0.5;  // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int gainCoeff = 300;      // Коэффициент усиления П регулятора при выравнивании после поворота

int walldistance = 9;     // расстояние до стены
bool maze=false;         // едем в лабиринте
bool driveForward = true;         // едем определенное расстояние

float Ei = 0;
float Ep;

float Kp; // пропорциональный
float Ki; // интегральный
float Kd; // диференциальный

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
  driveForward = false;
}

void loop()
{
  // ##### Тесты Начало ######
  //go(-baseSpeed, -baseSpeed);
  //test();
  //consoleLog();
  
  // ##### Тесты Конец ######1488

 // pid(baseSpeed);
//   Serial.print(isOnBlack(IR_SENSOR_L_PIN));
//   Serial.print(" ");
//     Serial.print(isOnBlack(IR_SENSOR_R_PIN));
// Serial.print(" ");
//     Serial.print(getIRSensorValue(IR_SENSOR_L_PIN));
//     Serial.print(" ");
//     Serial.println(getIRSensorValue(IR_SENSOR_R_PIN));
    
  // if (isOnCross())
  // {
    
    
    
  //   if (crossCount<4)
  //   {
  //     doezd();
  //     doezd();
  //    right();
  //     if (uzd(UZF_TRIGGER_PIN, UZF_ECHO_PIN)<50)
  //     {
  //       moveToTakeObjectOnBlack();
  //       left();
  //       left();
  //       moveToPutObjectOnBlack();
  //       right();
  //     }
      
  //  }
    
  // }
  
closeServo();
delay(1000);
openServo();
delay(1000);


}
