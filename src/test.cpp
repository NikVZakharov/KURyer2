#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <LCD.h>
#include <encoder.h>

void testEncoders()
{
    LCDprint(0, 0, getEncoder1());
    LCDprint(1, 0, getEncoder2());
  //  delay(baseDelay);
}

void testUZD()
{

    int result_f = getDistance(UZF_TRIGGER_PIN,UZF_ECHO_PIN);
    int result_s = getDistance(UZS_TRIGGER_PIN,UZS_ECHO_PIN);
    LCDprint(0, 0, result_f);
    LCDprint(1, 0, result_s);

}


void testServo()
{
    closeServo();
    delay(baseDelay);
    openServo();
}

void testIsOnBlack()
{
    int result_1 = isOnBlack(IR_SENSOR_L_PIN);
    int result_2 = isOnBlack(IR_SENSOR_R_PIN);
    LCDprint(0, 0, result_1);
    LCDprint(1, 0, result_2);
//    delay(baseDelay);
}

void testIRSensors()
{
    int result_1 = getIRSensorValue(IR_SENSOR_L_PIN);
    int result_2 = getIRSensorValue(IR_SENSOR_R_PIN);
    // LCDprint(0, 0, result_1);
    // LCDprint(1, 0, result_2);
    

//    delay(baseDelay);
}
void testIRSensorsAnalogRead()
{
    int result_1 =analogRead(IR_SENSOR_L_PIN);
    int result_2 = analogRead(IR_SENSOR_R_PIN);
    LCDprint(0, 0, result_1);
    LCDprint(1, 0, result_2);
//    delay(baseDelay);
}

void testIsOnCross()
{
    int result_1 = isOnCross();
     LCDprint(0, 0, result_1);
//    delay(baseDelay);
}

void testFullTurn()
{
    right();
    right();
    right();
    right();
}

void test()
{
    unsigned long testTime = 5000;
    // // ########### 1 движение вперед  #####################
    // LCDprint(0, 15, 1);
    //  LCDprint(0, 0, baseSpeed);
    // LCDprint(1, 0, baseSpeed);
    // go(baseSpeed, baseSpeed, testTime);
    // go(0, 0);
    // lcdclear();

    // // ########### 2 левое колесо назад правое вперед  #####################
    // LCDprint(0, 15, 2);
    // LCDprint(0, 0, -baseSpeed);
    // LCDprint(1, 0, baseSpeed);
    // go(-baseSpeed, baseSpeed, testTime);
    // go(0, 0);
    // lcdclear();

    // ########### 3 ИК сенсоры  #####################
    //LCDprint(0, 15, 3);
    startTime = millis();
    while (millis() - startTime < testTime)
    {
        testIRSensors();
    }
    // lcdclear();

//     //########### 4 сервопривод  #####################
//     LCDprint(0, 15, 4);
//     startTime = millis();
//     while (millis() - startTime < testTime)
//     {
//         testServo();
//     }
//     lcdclear();

//   //########### 5 УЗ датчики  #####################
//         LCDprint(0, 15, 5);
//         startTime = millis();
//         while (millis() - startTime < testTime)
//         {
//             testUZD();
//         }
//         lcdclear();
//    //########### 6 На перекрестке #####################
//         LCDprint(0, 15, 6);
//         startTime = millis();
//         while (millis() - startTime < testTime)
//         {
//             testIsOnCross();
//         }
//         lcdclear();
//    //########### 7 На черной линии #####################
//         LCDprint(0, 15, 7);
//         startTime = millis();
//         while (millis() - startTime < testTime)
//         {
//             testIsOnBlack();
//         }
//         lcdclear();
 //   ########### Энкодеры  #####################
    // LCDprint(0, 15, 8);
    // startTime = millis();
    // while (millis() - startTime < testTime)
    // {
    //     testEncoders();
    // }
    // lcdclear();


  //   preg(baseSpeed);

    // testFullTurn();
 //    testIRSensorsAnalogRead();
 //    testEncoders();

}
