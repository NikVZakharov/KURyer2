#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <uzd.h>
#include <middleware.h>
#include <lcd.h>
#include <maze.h>


void firstFieldAlgorithm() {
  static unsigned long lastStatusTime = 0;
  static unsigned long lastLogTime = 0;
  static unsigned long lastDebugPrintTime = 0;

  // Follow the line using PID control
  pid(baseSpeed);

  // Display status periodically if not in debug mode
  if (!DEBUG_MODE && millis() - lastStatusTime > 1000)
  {
    lcdShow(0, 0, "Following line");
    lastStatusTime = millis();
  }

  // Check for obstacles
  float frontDistance = getFDistance();

  // Display sensor values if in debug mode
  if (DEBUG_MODE && millis() - lastDebugPrintTime > 200)
  {
    int leftValue = getIRSensorValue(IR_SENSOR_L_PIN);
    int rightValue = getIRSensorValue(IR_SENSOR_R_PIN);
    int centerValue = getIRSensorValue(IR_SENSOR_S_PIN);

    lcdShow(0, 0, "L:");
    lcdShow(0, 2, leftValue);
    lcdShow(0, 7, "R:");
    lcdShow(0, 9, rightValue);

    lcdShow(1, 0, "C:");
    lcdShow(1, 2, centerValue);
    lcdShow(1, 7, "D:");
    lcdShow(1, 9, frontDistance);

    lastDebugPrintTime = millis();
  }
  // Log distance readings periodically to LCD if not in debug mode
  else if (!DEBUG_MODE && millis() - lastLogTime > 500)
  {
    lcdShow(1, 0, "Dist: ");
    lcdShow(1, 6, frontDistance);
    lastLogTime = millis();
  }

  // If obstacle detected
  if (frontDistance < distanceToCheckObject && frontDistance > 0)
  {
    if (isObezdDone)
    {
      return;
    }
    lcdShow(0, 0, "Obstacle found!");
    lcdShow(1, 0, "Dist: ");
    lcdShow(1, 6, frontDistance);

    avoidObstacle();

    // Reset timers after avoidance
    lastStatusTime = millis();
    lastLogTime = millis();
    lastDebugPrintTime = millis();
  }
  

  
  // Check if we've reached the end of the First Field
  if (crossCount >= 1) {
    stop();
    lcdclear();
    lcdShow(0, 0, "First Field done");
    lcdShow(1, 0, "Crosses: ");
    lcdShow(1, 9, crossCount);
    delay(2000);
    
    // Transition to the maze field
    lcdclear();
    lcdShow(0, 0, "First completed");
    lcdShow(1, 0, "Go to Maze Field");
    delay(1000);
    
    currentField = FIELD_MAZE;
    initMaze();
    maze = true;
    
    return;
  }
}

// Теперь функция firstFieldAlgorithm содержит всю необходимую логику для первого поля,
// включая обнаружение перекрестков, вывод информации на экран и переход к полю лабиринта


