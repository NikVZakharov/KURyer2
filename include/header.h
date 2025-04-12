#ifndef HEADER_H
#define HEADER_H

// Определение перечисления для отслеживания текущего поля
enum RobotField
{
  FIELD_LINE_FOLLOW = 0,
  FIELD_MAZE = 1,
  FIELD_THIRD = 2,
  FIELD_FINISH = 3
};

// Устанавливаем номера пинов
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN;
extern const int IR_SENSOR_S_PIN;
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
extern const int UZR_TRIGGER_PIN;
extern const int UZL_TRIGGER_PIN;




extern const bool IS_IR_SENSORS_REVERS;

extern const float KOEFF_FIX_MOTOR_L_SPEED;
extern const bool FIXPOSITION;
extern const int MAX_MOTOR_SPEED;
extern const int meanArraySize;
extern const float MAX_INTEGRAL;
extern const int START_WIDTH;
extern const int CROSS_WIDTH;
extern const int CUBE_COUNT;
extern const int CUBE_DISTANCE;
extern const int DISTANCE_TO_FIRST_CUBE;
extern const int WHITE_LINE_THRESHOLD; // Порог для обнаружения белой линии


extern int baseSpeed;
extern int minIRL, minIRR, maxIRL, maxIRR;
extern float koef_preg_p;
extern unsigned long baseDelay;
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
extern unsigned long testTime;
extern int otezdDelay;
extern int banka;
extern int walldistance;
extern bool maze;
extern bool driveDirect;
extern float Ei;
extern float Kp; // пропорциональный
extern float Ki; // интегральный
extern float Kd; // диференциальный
extern float Ep;
extern int wallDistance;
extern int distance;
extern int cubeColors[];
extern int cubeColorsMirror[];
extern int arrayCount;
extern int binary[];
extern int cubeColorsREV[];
extern int starshiy[];
extern bool whiteLineModeEnabled;     // Flag to control white/black line following mode
extern bool isObezdDone;  // проверка объезда банки
extern bool DEBUG_MODE; // Set to true to enable detailed debug information
extern RobotField currentField;
// extern int cube_cross; 
// extern int cubeCrossCount;

#endif