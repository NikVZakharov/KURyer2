#ifndef HEADER_H
#define HEADER_H
extern const int UZF_TRIGGER_PIN;
extern const int UZF_ECHO_PIN;
// Устанавливаем номера пинов для датчиков линии
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN;
extern const int IR_SENSOR_M_PIN;
extern const int MOTOR_L_DIRECTION_PIN;
extern const int MOTOR_L_SPEED_PIN;
extern const int MOTOR_R_DIRECTION_PIN;
extern const int MOTOR_R_SPEED_PIN;
extern const int SERVO_PIN;
extern const int FINISH_CROSS_COUNT;
extern const float KOEFF_FIX_MOTOR_L_SPEED;
extern const bool FIXPOSITION;
extern const int MAX_MOTOR_SPEED;

extern int baseSpeed;
extern int minIRL, minIRR, maxIRL, maxIRR;
extern float KOEF_ERROR;
extern int baseDelay;
extern int servoOpenPosition;
extern int servoClosePosition;
extern int blackLimit;
extern unsigned long startTime;
extern float timeToMoveBanka;
extern int distanceToTakeBanka;
extern int distanceToCheckBanka;
extern int timeToMoveBackWithBanka;
extern int crossCount;
extern int crossDelay;
extern bool haveBanka;
extern int gainCoeff;
extern int maxErrorTurnFix;
extern int obezdDelay;
extern int finishDelay;
extern int povorotDelay;

#endif