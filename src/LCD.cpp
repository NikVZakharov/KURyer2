#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>
#include <header.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void initLCD(){
   lcd.init();
  lcd.backlight();

}

void lcdclear(){
  lcd.clear();
}

void LCDprint(int x, int y, float number){

   startTime = millis();                            // Считываем текущее время
      while (millis() - startTime < timeToShowLED) // Пока текущее время - время старта таймера меньше интервала выравнивания едем по preg()
     {
      lcd.setCursor(y, x);      
      lcd.print(number);
     }
//delay(50);
lcdclear();
}

