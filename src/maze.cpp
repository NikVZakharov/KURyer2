#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <middleware.h>
#include <uzd.h>
#include <LCD.h>
#include <encoder.h>
#include <maze.h>
#include <third_field.h>

// Maze algorithm parameters
const int WALL_DETECTION_THRESHOLD = 15; // Distance in cm to detect walls
const float MAZE_SPEED = 80;            // Speed during maze navigation
const int WALL_TARGET_DISTANCE = 8;      // Целевое расстояние до стены в см
const int FRONT_WALL_MIN_DISTANCE = 12;  // Минимальное расстояние до передней стены перед поворотом

// PID регулятор для лабиринта
const float MAZE_KP = 3.0;  // Пропорциональный коэффициент (сильнее влияет на отклонения)
const float MAZE_KI = 0.0;  // Интегральный коэффициент (нулевой, чтобы не накапливать ошибку)
const float MAZE_KD = 1.0;  // Дифференциальный коэффициент (для сглаживания движений)

// Status tracking
bool mazeActive = false;
int mazeCellsTraversed = 0;
static unsigned long lastStatusTime = 0;
static unsigned long lastDisplayTime = 0;
static bool displayModeDetailed = false;  // Режим отображения: true = состояние движения, false = показания датчиков

// Переменная для отслеживания состояния в лабиринте
enum MazeState {
  FOLLOW_RIGHT_WALL,  // Следовать вдоль правой стены
  TURN_LEFT,          // Поворот налево
  TURN_RIGHT,         // Поворот направо 
  TURN_AROUND         // Разворот на 180°
};

// Текущее состояние лабиринта
MazeState currentMazeState = FOLLOW_RIGHT_WALL;

// Initialize maze navigation
void initMaze() {
  mazeActive = true;
  mazeCellsTraversed = 0;
  currentMazeState = FOLLOW_RIGHT_WALL;
  displayModeDetailed = false;  // true - показываем состояние движения, false - показания датчиков
  
  // Clear encoders for accurate measurement
  clearAllEncoders();
  
  // Display detailed status
  lcdclear();
  lcdShow(0, 0, "Maze Mode Start");
  if (displayModeDetailed) {
    lcdShow(1, 0, "Mode: Status");
  } else {
    lcdShow(1, 0, "Mode: Sensors");
  }
  delay(1000);
  
  // Активируем режим лабиринта для PID регулятора
  maze = true;
  
  lcdclear();
}

// Функция для переключения режима отображения
void toggleDisplayMode() {
  displayModeDetailed = !displayModeDetailed;
  lcdclear();
  
  lcdShow(0, 0, "Mode changed!");
  if (displayModeDetailed) {
    lcdShow(1, 0, "Status mode ON");
  } else {
    lcdShow(1, 0, "Sensor mode ON");
  }
  delay(1000);
  lcdclear();
}

// Check if at maze exit (using center IR sensor)
bool isMazeCompleted() {
  bool completed = (crossCount==2); // Detected black line indicating exit
  
  if (completed) {
    lcdclear();
    lcdShow(0, 0, "Exit detected!");
    lcdShow(1, 0, "Black line found");
    delay(500);
  }
  
  return completed;
}

// Вспомогательная функция для обновления экрана с текущим состоянием
void updateMazeDisplay(const char* line1 = "", const char* line2 = "") {
  static unsigned long lastDisplayUpdate = 0;
  static String lastLine1 = "";
  static String lastLine2 = "";
  
  // Обновляем дисплей только если изменился текст или прошло достаточно времени
  if (millis() - lastDisplayUpdate > 500 || 
      lastLine1 != line1 || 
      lastLine2 != line2) {
    
    lcdclear();
    if (strlen(line1) > 0) lcdShow(0, 0, line1);
    if (strlen(line2) > 0) lcdShow(1, 0, line2);
    
    lastLine1 = line1;
    lastLine2 = line2;
    lastDisplayUpdate = millis();
  }
}

void followWallWithPID() {
  // Периодически обновляем дисплей для отображения сенсоров
  static unsigned long lastDisplayTime = 0;
  
  // Получаем текущие расстояния до стен
  float frontDistance = getFDistance();
  float rightDistance = getRDistance();
  float leftDistance = getLDistance();
  
  // Обновляем информацию на дисплее раз в 500 мс
  if (millis() - lastDisplayTime > 500) {
    lcdclear();
    
    if (!displayModeDetailed) {
      // Режим 1: Показания датчиков
      lcdShow(0, 0, "SNS F:");
      lcdShow(0, 6, frontDistance);
      lcdShow(0, 11, "R:");
      lcdShow(0, 13, rightDistance);
      lcdShow(1, 0, "L:");
      lcdShow(1, 2, leftDistance);
      lcdShow(1, 7, "C:");
      lcdShow(1, 9, mazeCellsTraversed);
    } else {
      // Режим 2: Состояние движения
      if (maze) {
        lcdShow(0, 0, "STS Wall PID");
        lcdShow(0, 12, "D:");
        lcdShow(0, 14, rightDistance);
      } else if (driveDirect) {
        lcdShow(0, 0, "STS ENC PID");
        lcdShow(0, 12, "E:");
        lcdShow(0, 14, getEncoderError());
      } else {
        lcdShow(0, 0, "STS IR PID");
      }
      lcdShow(1, 0, "Cell:");
      lcdShow(1, 5, mazeCellsTraversed);
      lcdShow(1, 9, "S:");
      lcdShow(1, 11, MAZE_SPEED);
    }
    
    lastDisplayTime = millis();
  }
  
  // Проверяем нужно ли повернуть
  if (frontDistance > 0 && frontDistance < FRONT_WALL_MIN_DISTANCE) {
    // Останавливаемся для анализа и вывода отладочной информации
    stop();
    
    // Выводим информацию в соответствии с выбранным режимом
    lcdclear();
    if (!displayModeDetailed) {
      // Режим 1: Показания датчиков
      lcdShow(0, 0, "SNS F:");
      lcdShow(0, 6, frontDistance);
      lcdShow(0, 11, "R:");
      lcdShow(0, 13, rightDistance);
      lcdShow(1, 0, "L:");
      lcdShow(1, 2, leftDistance);
    } else {
      // Режим 2: Статус движения
      lcdShow(0, 0, "STS Wall detect!");
      lcdShow(1, 0, "Analyzing path...");
    }
    delay(500); // Пауза для чтения показаний
    
    // Если стена спереди близко, решаем в какую сторону повернуть
    if (leftDistance > WALL_DETECTION_THRESHOLD * 1.5) {
      // Если слева свободно, поворачиваем налево
      currentMazeState = TURN_LEFT;
      String leftMsg = "L open:" + String(leftDistance);
      updateMazeDisplay("Turn LEFT: F wall", leftMsg.c_str());
    } 
    else if (rightDistance > WALL_DETECTION_THRESHOLD * 1.5) {
      // Если справа свободно, поворачиваем направо
      currentMazeState = TURN_RIGHT;
      String rightMsg = "R open:" + String(rightDistance);
      updateMazeDisplay("Turn RIGHT: F wall", rightMsg.c_str());
    }
    else {
      // Если и слева и справа стены - тупик, разворачиваемся
      currentMazeState = TURN_AROUND;
      updateMazeDisplay("Turn AROUND", "All walls detected");
    }
    
    // Пауза для чтения информации о решении
     delay(300);
    
    // Выходим из функции, поворот будет выполнен в mazeAlgorithm
    return;
  }
  
  // Если поворачивать не нужно, продолжаем движение вперед с PID
  // Сохраняем предыдущие настройки режима
  bool prevMazeMode = maze;
  bool prevDriveDirectMode = driveDirect;
  
  if (rightDistance > 0 && rightDistance < WALL_DETECTION_THRESHOLD * 2) {
    // Если справа есть стена, используем режим следования вдоль стены
    maze = true;
    driveDirect = false;
    walldistance = WALL_TARGET_DISTANCE; // Целевое расстояние до стены
    
    // PID с использованием стены
    pid(MAZE_SPEED);
  } 
  else {
    // Если стены справа нет, используем режим движения по прямой с энкодерами
    maze = false;
    driveDirect = true;
    
    // PID с использованием энкодеров
    pid(MAZE_SPEED);
  }
  
  // Восстанавливаем предыдущие настройки режима
  maze = prevMazeMode;
  driveDirect = prevDriveDirectMode;
  
  // Проверяем, если мы проехали достаточно для засчитывания ячейки
  static unsigned long cellTimer = 0;
  if (millis() - cellTimer > 2000) { // Примерный интервал между ячейками
    mazeCellsTraversed++;
    cellTimer = millis();
    
    // Краткое уведомление о прохождении ячейки
    String cellMsg = "Total: " + String(mazeCellsTraversed);
    updateMazeDisplay("Cell reached", cellMsg.c_str());
  }
}

// Выполнение поворота с учетом текущего состояния
void executeTurn() {
  // Убедимся что робот точно остановился
  stop();
  delay(200);
  
  // Показываем предповоротную информацию
  lcdclear();
  
  // Получаем текущие расстояния до стен для отладки
  float frontDistance = getFDistance();
  float rightDistance = getRDistance();
  float leftDistance = getLDistance();
  
  // Информация о предстоящем повороте
  String turnInfo;
  
  // Выполняем поворот в соответствии с текущим состоянием
  switch (currentMazeState) {
    case TURN_LEFT:
      {
        turnInfo = "LEFT TURN";
        if (!displayModeDetailed) {
          String leftInfo = "SNS L:" + String(leftDistance);
          updateMazeDisplay(("SNS " + turnInfo).c_str(), leftInfo.c_str());
        } else {
          updateMazeDisplay(("STS " + turnInfo).c_str(), "Front wall detected");
        }
        delay(500);
        turnEnc(MAZE_SPEED, -90);  // Отрицательный угол - поворот влево
        mazeCellsTraversed++;
      }
      break;
      
    case TURN_RIGHT:
      {
        turnInfo = "RIGHT TURN";
        if (!displayModeDetailed) {
          String rightInfo = "SNS R:" + String(rightDistance);
          updateMazeDisplay(("SNS " + turnInfo).c_str(), rightInfo.c_str());
        } else {
          updateMazeDisplay(("STS " + turnInfo).c_str(), "Front wall detected");
        }
        delay(500);
        turnEnc(MAZE_SPEED, 90);   // Положительный угол - поворот вправо
        mazeCellsTraversed++;
      }
      break;
      
    case TURN_AROUND:
      {
        turnInfo = "U-TURN";
        if (!displayModeDetailed) {
          String frontInfo = "SNS F:" + String(frontDistance);
          updateMazeDisplay(("SNS " + turnInfo).c_str(), frontInfo.c_str());
        } else {
          updateMazeDisplay(("STS " + turnInfo).c_str(), "Dead end detected");
        }
        delay(500);
        turnEnc(MAZE_SPEED, 180);  // Разворот на 180 градусов
        mazeCellsTraversed++;
      }
      break;
      
    default:
      break;
  }
  
  // После поворота продолжаем следовать вдоль стены
  currentMazeState = FOLLOW_RIGHT_WALL;
  
  // Остановка после поворота для проверки положения
  stop();
  
  // Обновляем показания датчиков после поворота
  frontDistance = getFDistance();
  rightDistance = getRDistance();
  leftDistance = getLDistance();
  
  // Выводим информацию о положении после поворота
  lcdclear();
  if (!displayModeDetailed) {
    lcdShow(0, 0, "SNS After turn:");
    char afterTurnInfo[16]; // Буфер для информации
    sprintf(afterTurnInfo, "F:%d R:%d", (int)frontDistance, (int)rightDistance);
    lcdShow(1, 0, afterTurnInfo);
  } else {
    lcdShow(0, 0, "STS Turn done");
    lcdShow(1, 0, "Ready to continue");
  }
  delay(300);
  
  updateMazeDisplay("Continue", "Following wall");
}

// Main maze algorithm function
void mazeAlgorithm() {
  // Если режим лабиринта не активен, инициализируем
  if (!mazeActive) {
    updateMazeDisplay("Starting maze", "navigation...");
    delay(300);
    
    initMaze();
  }
  
  // Проверяем, достигли ли выхода из лабиринта
  if (isMazeCompleted()) {
    stop();
    updateMazeDisplay("Maze completed!", "");
    delay(500);
    
    // Отображаем статистику
    String cellMsg = "Cells: " + String(mazeCellsTraversed);
    updateMazeDisplay("Statistics:", cellMsg.c_str());
    delay(1000);
    
    mazeActive = false;
    maze = false; // Выключаем режим лабиринта
    
    // Переход к третьему полю
    updateMazeDisplay("Maze completed", "Go to Third Field");
    delay(500);
    
    currentField = FIELD_THIRD;
    initThirdField();
    
    return;
  }
  
  // Выполняем алгоритм в зависимости от текущего состояния
  switch (currentMazeState) {
    case FOLLOW_RIGHT_WALL:
      // Непрерывное движение с PID вдоль правой стены
      followWallWithPID();
      break;
      
    case TURN_LEFT:
    case TURN_RIGHT:
    case TURN_AROUND:
      // Выполняем поворот
      executeTurn();
      break;
  }
} 