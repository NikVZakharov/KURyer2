#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>
#include <header.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD()
{
  lcd.init();
  lcd.backlight();
}

void lcdclear()
{
  lcd.clear();
}

void LCDprint(int x, int y, float number)
{
  //lcd.clear();
  startTime = millis();                        // Считываем текущее время
  while (millis() - startTime < timeToShowLED) // Пока текущее время - время старта таймера меньше интервала выравнивания едем
  {
    lcd.setCursor(y, x);
    lcd.print(number);
  }
  // delay(50);

}
