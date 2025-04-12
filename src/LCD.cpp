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

void lcdShow(int x, int y, const char *text)
{
  lcd.setCursor(y, x);
  lcd.print(text);
}

void lcdShow(int x, int y, float number)
{
  lcd.setCursor(y, x);
  lcd.print(number);
}
