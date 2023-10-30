#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>

void test()
{

    go(baseSpeed, baseSpeed); //тест моторов
    
 //   testUZDF(); // тест переднего УЗ датчика

 //   testServo() //тест сервопривода
}

int testUZDF()
{

    return uzdF();
}

void testServo()
{
 //   closeServo();
    delay(baseDelay);
 //   openServo();
    delay(baseDelay);
}