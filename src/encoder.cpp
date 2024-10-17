

#include <arduino_encoder.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <Arduino.h>
#include <header.h>
#include <LCD.h>




Encoder enc1;
Encoder enc2;

void initENC(){
  enc1.setup(MOTOR_L_ENCODER_PIN1, MOTOR_L_ENCODER_PIN2);
  enc2.setup(MOTOR_R_ENCODER_PIN1, MOTOR_R_ENCODER_PIN2);
  enc1.reverse();
  enc2.reverse();
}

long getEncoder1(){
long  value=enc1.get();
//LCDprint(0, 0, value);
  return value;
  
}

long getEncoder2(){
long  value=enc2.get();
//LCDprint(1, 0, value);
  return value;
  
}

void clearEncoder1(){

   enc1.clear();
  
}

void clearEncoder2(){

   enc2.clear();
  
}

void clearAllEncoders(){

  clearEncoder1();
  clearEncoder2();
}