#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>

#include <go.h>
#include <preg.h>
#include <right.h>
#include <left.h>
#include <UZDL.h>
#include <UZDF.h>
#include <header.h>

Servo servo;
const int UZF_TRIGGER_PIN = 7;
const int UZF_ECHO_PIN = 8;

int V = 150;
int min1 = 400;
int max1 = 600;
int min2 = 400;
int max2 = 600;
float K = 0.4;
int N = 0;
int N1 = 0;
int VP = 90;
int servoOpenPosition = 35;
int servoClosePosition = 90;

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
  Serial.begin(9600); // включаем монитор порта
}

bool isOnCross()
{
  bool result = false;
  if (analogRead(A0) < 500 && analogRead(A1) < 500)
  {
    result = true;
  }
  return result;
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
  preg();
  Serial.println(uzdF());
  if (isOnCross())
  {
    go(V, V);
    delay(1000);
    go(0, 0);
    delay(1000);
    preg();
    N = N + 1;
    if (N == 2)
    {
      go(0, 0);
      delay(2000);
      right();
      delay(1000);
      
      while (uzdF() > 7)
      {
        preg();
        if (isOnCross())
        {
          go(V,V);
        }
        
      }
      go(0,0);
      delay(1000);
      closeServo();
      go(0, 0);
      delay(1000);
      go(-V, -V);
      delay(1500);
      right();
      while (1)
      {
        /* code */
      }

  //     //     while (N1 < 2)
  //     //     {
  //     //       preg();
  //     //       if (isOnCross())
  //     //       {
  //     //         N1 = N1 + 1;
  //     //       }
  //     //     }
  //     //     openServo();
  //     //     while (1)
  //     //     {
  //     //       /* code */
  //     //     }
    }
   }
}

// for(int i=120;i>50;i--) {shwat.write(i);delay(20);}