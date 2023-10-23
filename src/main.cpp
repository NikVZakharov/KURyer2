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

Servo shwat;

int V = 60;
int min1 = 400;
int max1 = 600;
int min2 = 400;
int max2 = 600;
float K = 0.3;
int N=0;
int VP=90;



void setup()
{
  pinMode(2, OUTPUT); // напр. мотора лев.
  pinMode(3, OUTPUT); // скор. мотора лев.
  pinMode(4, OUTPUT); // напр. мотора прав.
  pinMode(5, OUTPUT); // скор. мотора прав.
  pinMode(7, OUTPUT); // пинок дальномера uzdL - левого
  pinMode(9, OUTPUT); // пинок дальномера uzdF - фронтального
  pinMode(8, INPUT);  // эхо-прием дальномера uzdL - левого
  pinMode(10, INPUT); // эхо-прием дальномера uzdF - правого
  pinMode(A0, INPUT); // датчик ИК - А0
  pinMode(A1, INPUT); // датчик ИК - А1
  pinMode(A2, INPUT); // датчик ИК - А2
  shwat.attach(11);
  shwat.write(50);
  Serial.begin(9600); // включаем монитор порта
}

void loop()
{
  preg();
  
  int D0=analogRead(A0);
  int D1=analogRead(A1);
      Serial.print(D0);
      Serial.print(" ");
    Serial.println(D1);
 
 if (D0 < 400 && D1 < 400)
 {
    
    go(0,0);
    delay(1000);
    right();
    go(0,0);
    while (1)
    {
      /* code */
    }
    
   
 }
 
}


// for(int i=120;i>50;i--) {shwat.write(i);delay(20);}