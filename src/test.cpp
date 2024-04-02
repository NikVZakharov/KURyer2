#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <LCD.h>

int testUZDF()
{
    int result=uzdF();
    LCDprint(0, 0, result);
    return result;
}

void testServo()
{
    closeServo();
    delay(baseDelay);
    openServo();
}

void testIsOnBlack(){
    LCDprint(0, 0, isOnBlack(IR_SENSOR_L_PIN));
    LCDprint(1, 0, isOnBlack(IR_SENSOR_R_PIN));
    delay(baseDelay);
}

void testIRSensors(){
    LCDprint(0, 0, analogRead(IR_SENSOR_L_PIN));
    LCDprint(1, 0, analogRead(IR_SENSOR_R_PIN));
    delay(baseDelay);
}

void testIsOnCross(){
    LCDprint(0, 0, isOnCross()); 
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
 //go(-baseSpeed, baseSpeed); //тест моторов
preg(baseSpeed);
// testUZDF(); // тест переднего УЗ датчика
// testServo(); //тест сервопривода
// testIsOnBlack();
// testIsOnCross();
// testFullTurn();
// testIRSensors();
}

