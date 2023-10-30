#ifndef HEADER_H
#define HEADER_H
extern const int UZF_TRIGGER_PIN;
extern const int UZF_ECHO_PIN;
// Устанавливаем номера пинов для датчиков линии
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN;
extern const int DVIG_L_MOTOR_PIN;
extern const int SPEED_DVIG_L_MOTOR_PIN;
extern const int DVIG_R_MOTOR_PIN;
extern const int SPEED_DVIG_R_MOTOR_PIN;
extern const int SERVO_PIN;

extern int baseSpeed;
extern int minIRL, minIRR , maxIRL, maxIRR ;
extern float KOEF_ERROR;
extern int baseDelay;
extern int servoOpenPosition;
extern int servoClosePosition;

#endif