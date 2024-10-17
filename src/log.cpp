
#include <Arduino.h>
#include <drive.h>
#include <middleware.h>
#include <UZD.h>
#include <header.h>
#include <encoder.h>

void logInit()
{
    Serial.begin(9600); // Only using Serial when not debugging!
}

void consoleLog(int interval=500)
{

    //     //значения переднего УЗ датчика
    //     Serial.println(uzdF());

    // //     // значения левого и правого ИК датчика
    // Serial.println(analogRead(IR_SENSOR_L_PIN));
    // Serial.print(" ");
    // Serial.print(analogRead(IR_SENSOR_R_PIN));
    // Serial.print(" ");

    // //    // значения левого и правого ИК датчика нормализованные
    // Serial.print(getIRSensorValue(IR_SENSOR_L_PIN));
    // Serial.print(" ");
    // Serial.print(getIRSensorValue(IR_SENSOR_R_PIN));
    // Serial.println(" ");

    // //     //минимальные и мксимальные значения показаний левого ИК датчика
    // Serial.print(minIRL);
    // Serial.print(" ");
    // Serial.print(maxIRL);
    // Serial.print(" ");

    // // //минимальные и мксимальные значения показаний правого ИК датчика
    // Serial.print(minIRR);
    // Serial.print(" ");
    // Serial.print(maxIRR);
    // Serial.println(" ");

     //   ########### Энкодеры  #####################
    // Serial.print(getEncoder1());
    // Serial.print("  ");
    // Serial.println(getEncoder2());

        
    //Serial.println(isOnCross());

    //delay(interval);
}
