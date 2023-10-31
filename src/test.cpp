#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>

int testUZDF()
{
    int result=uzdF();
    Serial.println(result);
    return result;
}

void testServo()
{
    closeServo();
    openServo();
}

int testIsOnBlack(){
    Serial.print(isOnBlack(IR_SENSOR_L_PIN));
    Serial.print(" ");
    Serial.print(isOnBlack(IR_SENSOR_M_PIN));
    Serial.print(" ");
    Serial.println(isOnBlack(IR_SENSOR_R_PIN));
    delay(baseDelay);
}

int testIsOnCross(){
    Serial.println(isOnCross());
    delay(baseDelay);
}

int testTurn(){
    right();
    right();
    right();
    right();
}


void test()
{

     //go(baseSpeed, -baseSpeed); //тест моторов

    //preg();

     //testUZDF(); // тест переднего УЗ датчика

     //  testServo(); //тест сервопривода
       //testIsOnBlack();
    //   testIsOnCross();
   // testTurn();
}

