#ifndef LCD_h
#define LCD_h

void initLCD();
void lcdShow(int x, int y, const char *text);
void lcdShow(int x, int y, float number);
void lcdclear();
#endif