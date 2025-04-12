#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <servoMotor.h>
#include <LCD.h>
#include <encoder.h>

// void testEncoders()
// {
//     lcdShow(0, 0, getEncoder1());
//     lcdShow(1, 0, getEncoder2());
//     //  delay(baseDelay);
// }

void testUZD()
{
    int result_l = getLDistance();
    int result_f = getFDistance();
    int result_r = getRDistance();

    // lcdShow(0, 0, result_l);
    // lcdShow(1, 0, result_f);
    // lcdShow(1, 0, result_r);
    Serial.print(result_l);
    Serial.print("   ");
    Serial.print(result_f);
    Serial.print("   ");
    Serial.println(result_r);
}

// void testServo()
// {
//     closeServo();
//     delay(baseDelay);
//     openServo();
//     delay(baseDelay);
// }

void testIsOnBlack()
 {
//     int result_1 = isOnBlack(IR_SENSOR_L_PIN);
//     int result_2 = isOnBlack(IR_SENSOR_R_PIN);
    // lcdShow(0, 0, result_1);
    // lcdShow(1, 0, result_2);
    //    delay(baseDelay);
}

// void testIRSensors()
// {
//     int result_1 = getIRSensorValue(IR_SENSOR_L_PIN);
//     int result_2 = getIRSensorValue(IR_SENSOR_R_PIN);
//     Serial.print(result_1);
//     Serial.print("   ");
//     Serial.println(result_2);
//     // lcdShow(0, 0, result_1);
//     // lcdShow(1, 0, result_2);

//     //    delay(baseDelay);
// }
void testIRSensorsAnalogRead()
{
    // int result_1 = analogRead(IR_SENSOR_L_PIN);
    // int result_2 = analogRead(IR_SENSOR_R_PIN);
    // lcdShow(0, 0, result_1);
    // lcdShow(1, 0, result_2);
    //    delay(baseDelay);
}

// bool testIsOnCross()
// {
//     int result_1 = isOnCross();
//     //  lcdShow(0, 0, result_1);
//     //    delay(baseDelay);
//     return result_1;
// }

// void testFullTurn()
// {
//     right();
//     right();
//     right();
//     right();
// }

void test()
{
    // // unsigned long testTime = 100000;
    //  // ########### 1 движение вперед  #####################
    //  lcdShow(0, 15, 1);
    //   lcdShow(0, 0, baseSpeed);
    //  lcdShow(1, 0, baseSpeed);
    //  go(baseSpeed, baseSpeed);
    //  delay(1000);
    //  stop();
    //  lcdclear();

    // // ########### 2 левое колесо назад правое вперед  #####################
    // lcdShow(0, 15, 2);
    // lcdShow(0, 0, -baseSpeed);
    // lcdShow(1, 0, baseSpeed);
    // go(-baseSpeed, -baseSpeed);
    // delay(1000);
    // stop();
    // lcdclear();

    // // ########### 3 движение вперед  #####################
    //  lcdShow(0, 15, 1);
    //   lcdShow(0, 0, baseSpeed);
    //  lcdShow(1, 0, baseSpeed);
    //  go(-baseSpeed, baseSpeed);
    //  delay(1000);
    //  stop();
    //  lcdclear();

    // // ########### 4 левое колесо назад правое вперед  #####################
    // lcdShow(0, 15, 2);
    // lcdShow(0, 0, -baseSpeed);
    // lcdShow(1, 0, baseSpeed);
    // go(baseSpeed, -baseSpeed);
    // delay(1000);
    // stop();
    // lcdclear();

    //########### 3 ИК сенсоры  #####################
    // lcdShow(0, 15, 3);
    // startTime = millis();
    // while (millis() - startTime < testTime)
    // {
    //     testIRSensors();
    // }
    // lcdclear();

    // //########### pid  #####################
    // lcdShow(0, 15, 3);
    // startTime = millis();
    // while (millis() - startTime < testTime)
    // {
    //     pid(baseSpeed);
    // }
    // lcdclear();

    //     //########### 4 сервопривод  #####################
    // lcdShow(0, 15, 4);
    // startTime = millis();
    // while (millis() - startTime < testTime)
    // {
    //     testServo();
    // }
    // lcdclear();

    // //   //########### 5 УЗ датчики  #####################
    //         lcdShow(0, 15, 5);
    //         startTime = millis();
    //         while (millis() - startTime < testTime)
    //         {
                testUZD();
    //         }
    //         lcdclear();
    //    //########### 6 На перекрестке #####################
    //         lcdShow(0, 15, 6);
    //         startTime = millis();
    //         while (millis() - startTime < testTime)
    //         {
    //             testIsOnCross();
    //         }
    //         lcdclear();
    //    //########### 7 На черной линии #####################
    //         lcdShow(0, 15, 7);
    //         startTime = millis();
    //         while (millis() - startTime < testTime)
    //         {
    //             testIsOnBlack();
    //         }
    //         lcdclear();
    //   ########### Энкодеры  #####################
    // lcdShow(0, 15, 8);
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
