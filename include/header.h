#ifndef HEADER_H
#define HEADER_H
extern const int UZF_TRIGGER_PIN;
extern const int UZF_ECHO_PIN;
// Устанавливаем номера пинов для датчиков линии
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN;

extern int baseSpeed;
extern int minIRL, minIRR , maxIRL, maxIRR ;
extern int min1;
extern int max1;
extern int min2;
extern int max2;
extern float K;
extern int N;
extern int VP;
extern int baseDelay;

#endif