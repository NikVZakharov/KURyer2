#include <Wire.h>
#include <Arduino.h>
#include <NewPing.h>
#include <SPI.h>
#include <Adafruit_TCS34725.h>

#include <drive.h>
#include <middleware.h>
#include <uzd.h>
#include <header.h>
#include <servoMotor.h>
#include <banka.h>
#include <LCD.h>
#include <encoder.h>
#include <RGB.h>
#include <maze.h>
#include <third_field.h>
#include <first_field.h>
#include <log.h>
#include <test.h>

// ################## Константы ###################
// ################### Настройка пинов #############

const int MOTOR_L_DIRECTION_PIN = 2;
const int MOTOR_L_SPEED_PIN = 3;
const int MOTOR_R_DIRECTION_PIN = 4;
const int MOTOR_R_SPEED_PIN = 5;
const int MOTOR_R_ENCODER_PIN1 = 8;
const int MOTOR_R_ENCODER_PIN2 = 9;
const int MOTOR_L_ENCODER_PIN1 = 10;
const int MOTOR_L_ENCODER_PIN2 = 11;
const int IR_SENSOR_L_PIN = A0;
const int IR_SENSOR_R_PIN = A1;
const int IR_SENSOR_S_PIN = A2;

const int UZR_TRIGGER_PIN = 7;
const int UZF_TRIGGER_PIN = 6;
const int UZL_TRIGGER_PIN = 13;

const int LCD_SDA = A4;
const int LCD_SCL = A5;
const int SERVO_PIN = 13;

// ################### Настройки программы ############

const bool IS_IR_SENSORS_REVERS = false;

// const float KOEFF_FIX_MOTOR_L_SPEED = 0.8;
// const bool FIXPOSITION = true; // выравниваемся на повороте или нет
const int MAX_MOTOR_SPEED = 250;

const float MAX_INTEGRAL = 100; // Максимальное значение интегральной составляющей для защиты от wind-up
const int CROSS_WIDTH = 600;    // ширина линии перекрестка
const int START_WIDTH = 1500;   // длина старта
const int CUBE_COUNT = 10;      // количество кубов
const int CUBE_DISTANCE = 515;  // расстояние между кубиками
                                // место где линия переворачивания

// White line detection parameters
const int WHITE_LINE_THRESHOLD = 800; // Threshold for detecting white line
bool whiteLineModeEnabled = false;    // Flag to control white/black line following mode

// ############## Переменные ####################

// Базовые параметры
int baseSpeed = 150; // базовая скорость
int minIRL = 200, minIRR = 200, maxIRL = 800, maxIRR = 800;
unsigned long baseDelay = 55;  // задержка между действиями
unsigned long startTime = 0;   // Время начала таймера
int finishDelay = 1400;        // задержка при финишировании
int timeToShowLED = 10;        // время вывода информации на lcd дисплей
unsigned long testTime = 1000; // время для тестирования функции
int blackLimit = 600;          //   все что ниже-черная линия
bool isObezdDone = false;      // флаг объезда банки

// UZD



// П регулятор
float koef_preg_p = 0.5; // уменьшаем или увеличиваем ошибку чтобы не колбасило робота
int gainCoeff = 300;     // Коэффициент усиления П регулятора при выравнивании после поворота

int walldistance = 9;     // расстояние до стены
bool maze = false;        // едем в лабиринте
bool driveDirect = false; // едем определенное расстояние

float Ei = 0;
float Ep;

float Kp = 0.3; // пропорциональный
float Ki = 0;   // интегральный
float Kd = 5;   // диференциальный

// Сервопривод
int servoOpenPosition = 20;  // градус открытого серво
int servoClosePosition = 90; // градус закытого серво

// Перекрестки
int crossCount = 0;   // количество перекрестков
int crossDelay = 300; // то сколько проедет робот после того как датчики увидят перекресток

// Работа с банкой
int timeToMoveBackWithBanka = 1000;  // время, которое робот едет назад с банкой
unsigned long timeToMoveBanka = 400; // Время в течении которого выравниваем машину после поворота
unsigned long timeToMoveBanka2 = 2000;
int distanceToTakeBanka = 4;   // расстояние на котром надо взять банку
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
int distanceToCheckObject = 25; // расстояние на котором ищем объект при объезде

// Финиш
int FINISH_CROSS_COUNT = 21; // номер перекрестка на которам надо финишировать
int finishcount = 0;         // увеличиваем crosscount при случае если надо финишировать на определенном перекрестке

int whiteEdgeLimit = 550;
int blackEdgeLimit = 150;
int wallPosition = 1;

bool coordinatesSet = false;
int maxn = 0;
bool coordset = false;
int distance = 300;
int loopCount = 0;
int temp;

int cubeColors[CUBE_COUNT] = {0, 2, 1, 3, 0, 2, 2, 0, 1, 3};
// int cubeColors[CUBE_COUNT];
int cubeColorsMirror[CUBE_COUNT];
int cubeColorsREV[CUBE_COUNT];
int binary[CUBE_COUNT];
int starshiy[7] /* = {0, 0, 0, 0, 0, 0, 0}*/;
int arrayCount = 2;
bool direction = true;

// Debug mode toggle
bool DEBUG_MODE = false; // Set to true to enable detailed debug information

// Field tracking - используем RobotField, определенный в header.h
RobotField currentField = FIELD_LINE_FOLLOW;

// Function to toggle debug mode (can be called from other parts of the code)
void toggleDebugMode()
{
  DEBUG_MODE = !DEBUG_MODE;
  lcdclear();
  lcdShow(0, 0, DEBUG_MODE ? "Debug: ON" : "Debug: OFF");
  delay(1000);
}

void setup()
{
  // put your setup code here, to run once: v

  pinMode(MOTOR_L_DIRECTION_PIN, OUTPUT); // напр. мотора лев.
  pinMode(MOTOR_L_SPEED_PIN, OUTPUT);     // скор. мотора лев.
  pinMode(MOTOR_R_DIRECTION_PIN, OUTPUT); // напр. мотора прав.
  pinMode(MOTOR_R_SPEED_PIN, OUTPUT);     // скор. мотора прав.

  pinMode(UZF_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdF - фронтального
  pinMode(UZR_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdR - бокового
  pinMode(UZL_TRIGGER_PIN, OUTPUT); // пинок дальномера uzdL - бокового
  pinMode(IR_SENSOR_L_PIN, INPUT);  // пин левого датчика ИК
  pinMode(IR_SENSOR_R_PIN, INPUT);  // пин правого датчика ИК

  initLCD();
  initENC();
  initServo();
  logInit();

  // Set maze flag for PID controller
  maze = false; // Initially we're not in maze mode

  // Set initial field to line following
  currentField = FIELD_LINE_FOLLOW;

  // Display startup message
  lcdclear();
  lcdShow(0, 0, "Line Follow Mode");
  delay(1000);
  lcdclear();
}

void loop()
{
    if (isOnCross())
  {
    crossCount++;
  }
  // Process different fields based on current state
  switch (currentField) {
    case FIELD_LINE_FOLLOW:
      firstFieldAlgorithm();
      break;
      
    case FIELD_MAZE:
      // Run maze algorithm
      mazeAlgorithm();
      break;
      
    case FIELD_THIRD:
      // Run third field algorithm
      thirdFieldAlgorithm();
      break;
      
    case FIELD_FINISH:
      // Perform finish routine
      lcdclear();
      lcdShow(0, 0, "Finished!");
      finish();
      break;
  }
//test();

}
