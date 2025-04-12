#include <Arduino.h>
#include <header.h>
#include <log.h>
#include <Adafruit_TCS34725.h>

// Инициализация датчика TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// Порог для определения доминирующего цвета
const int colorThreshold = 20;   // Минимальное значение для красного и зелёного каналов
const int ratioThreshold = 30;   // Максимальная разница между красным и зелёным для жёлтого
const int clearThreshold = 200;  // Порог clear для определения присутствия объекта
const int blueThreshold = 100;   // Минимальное значение B для распознавания синего цвета
const int yellowBThreshold = 40; // Максимальное значение B для распознавания жёлтого цвета

// Функция для определения цвета
int detectColor(float red, float green, float blue, unsigned int clear, int colorThreshold, int ratioThreshold, int yellowBThreshold, int blueThreshold)
{
    // 1) Если clear слишком маленький — считаем, что объекта нет
    if (clear < clearThreshold)
    {
        return 0; // ПРЯМО
    }

    // Проверка на жёлтый цвет
    if ((red > colorThreshold) && (green > colorThreshold) && (abs(red - green) < ratioThreshold))
    {
        if (blue < yellowBThreshold)
        {
            return 1;  // НАЛЕВО
        }
    }
    // Проверка на красный цвет
    else if ((red > green + ratioThreshold) && (red > blue + ratioThreshold))
    {
        return 2;     // РАЗВОРОТ НА 180
    }
    // Проверка на синий цвет
    else if ((blue > red) && (blue > green))
    {
        return 3;     // НАПРАВО
    }
    else if ((green > blue + colorThreshold) && (green > red + colorThreshold) && (clear > 700))
    {
        return 4;     // РАЗВОРОТ НА 180
    }
    

    return 0;         // ПРЯМО
} 

int colorObject()
{
    uint16_t clear, red, green, blue;
    // startTime = millis();                     // Считываем текущее время
    // while (millis() - startTime < 100) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
    // {
    //     true;
    // }
     // "промывка" – первое чтение выкидываем
    tcs.getRawData(&red, &green, &blue, &clear);
    delay(30);
    // Чтение данных с датчика
    tcs.getRawData(&red, &green, &blue, &clear);

    // Вывод сырых значений
    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(" | Green: ");
    Serial.print(green);
    Serial.print(" | Blue: ");
    Serial.print(blue);
    Serial.print(" | Clear: ");
    Serial.println(clear);

    // Проверка на нулевое значение clear, чтобы избежать деления на ноль
    if (clear == 0)
    {
        clear = 1;
    }

    // Нормализация значений RGB на основе clear
    float r = red / (float)clear * 256.0;
    float g = green / (float)clear * 256.0;
    float b = blue / (float)clear * 256.0;

    // Ограничение значений в диапазон 0-255
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    // Вывод отмасштабированных значений цвета
    Serial.print("R = ");
    Serial.print(r);
    Serial.print(" | G = ");
    Serial.print(g);
    Serial.print(" | B = ");
    Serial.println(b);

    // Объект присутствует, распознаём цвет
    int color = detectColor(r, g, b, clear, colorThreshold, ratioThreshold, yellowBThreshold, blueThreshold);

    // Определение цвета
    Serial.println(color);

    startTime = millis();                     // Считываем текущее время
    while (millis() - startTime < 100) // Пока текущее время - время старта таймера меньше заданного интервала едем по preg()
    {
        true;
    }
    
    return color;
}