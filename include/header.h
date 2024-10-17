#ifndef HEADER_H
#define HEADER_H
// Устанавливаем номера пинов
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN;
extern const int IR_SENSOR_M_PIN;
extern const int MOTOR_L_DIRECTION_PIN;
extern const int MOTOR_L_SPEED_PIN;
extern const int MOTOR_R_DIRECTION_PIN;
extern const int MOTOR_R_SPEED_PIN;
extern const int SERVO_PIN;
extern const int MOTOR_L_ENCODER_PIN1;
extern const int MOTOR_L_ENCODER_PIN2;
extern const int MOTOR_R_ENCODER_PIN1;
extern const int MOTOR_R_ENCODER_PIN2;
extern const int UZF_TRIGGER_PIN;
extern const int UZF_ECHO_PIN;
extern const int UZS_TRIGGER_PIN;
extern const int UZS_ECHO_PIN;

extern const bool IS_IR_SENSORS_REVERS;

extern const float KOEFF_FIX_MOTOR_L_SPEED;
extern const bool FIXPOSITION;
extern const int MAX_MOTOR_SPEED;
extern const int meanArraySize;
extern const float MAX_INTEGRAL;
extern const int START_WIDTH;
extern const int CROSS_WIDTH;

extern int baseSpeed;
extern int minIRL, minIRR, maxIRL, maxIRR;
extern float koef_preg_p;
extern int baseDelay;
extern int servoOpenPosition;
extern int servoClosePosition;
extern int blackLimit;
extern unsigned long startTime;
extern unsigned long timeToMoveBanka;
extern unsigned long timeToMoveBanka2;
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
extern int povorotDelay2;
extern int obezdObjectDelay;
extern int distanceToCheckObject;
extern int whiteEdgeLimit;
extern int blackEdgeLimit;
extern int wallPosition;
extern int FINISH_CROSS_COUNT;
extern int finishcount;
extern int timeToShowLED;
extern int testTime;
extern int otezdDelay;
extern int banka;
extern int walldistance;
extern float pastUZDFValue[];
extern float pastUZDSValue[];
extern bool maze;
extern bool driveForward;
extern float Ei;
extern float Kp;  // пропорциональный
extern float Ki;   // интегральный
extern float Kd; // диференциальный
extern float Ep;
extern int wallDistance ;



#endif