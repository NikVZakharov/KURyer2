

#include <arduino_encoder.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <Arduino.h>
#include <header.h>
#include <LCD.h>

Encoder encL;
Encoder encR;

void initENC()
{
  encL.setup(MOTOR_L_ENCODER_PIN1, MOTOR_L_ENCODER_PIN2);
  encR.setup(MOTOR_R_ENCODER_PIN1, MOTOR_R_ENCODER_PIN2);
  encL.reverse();
  encR.reverse();
}

long getEncoderL()
{
  long value = encL.get();
  // lcdShow(0, 0, value);
  //Serial.print(value);
  //Serial.print(" ");
  return value;
}

long getEncoderR()
{
  long value = -encR.get();
  // lcdShow(1, 0, value);
  //Serial.println(value);
  return value;
}

void clearEncoderL()
{

  encL.clear();
}

void clearEncoderR()
{

  encR.clear();
}

void clearAllEncoders()
{

  clearEncoderL();
  clearEncoderR();
}