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
    delay(baseDelay);
    openServo();
}

void testIsOnBlack(){
    Serial.print(isOnBlack(IR_SENSOR_L_PIN));
    Serial.print(" ");
    Serial.print(isOnBlack(IR_SENSOR_M_PIN));
    Serial.print(" ");
    Serial.println(isOnBlack(IR_SENSOR_R_PIN));
    delay(baseDelay);
}

void testIRSensors(){
    Serial.print(analogRead(IR_SENSOR_L_PIN));
    Serial.print(" ");
    Serial.print(analogRead(IR_SENSOR_M_PIN));
    Serial.print(" ");
    Serial.println(analogRead(IR_SENSOR_R_PIN));
    delay(baseDelay);
}

void testIsOnCross(){
    Serial.println(isOnCross());
    delay(baseDelay);
}



void testFullTurn(){
    right();
    right();
    right();
    right();
}


void test()
{
//testIRSensors();
    // go(baseSpeed, baseSpeed); //тест моторов

    //preg(baseSpeed);

     //testUZDF(); // тест переднего УЗ датчика

    //   testServo(); //тест сервопривода
       //testIsOnBlack();
      // testIsOnCross();
    testFullTurn();
}

