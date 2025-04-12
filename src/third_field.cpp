#include <Arduino.h>
#include <header.h>
#include <drive.h>
#include <middleware.h>
#include <LCD.h>
#include <encoder.h>
#include <third_field.h>

// Third field parameters
const int THIRD_FIELD_TRANSITION_COUNT = 5;  // Number of transitions before completing the field
const int COLOR_SAMPLE_COUNT = 10;           // Number of samples to take for background detection
const int COLOR_DETECTION_THRESHOLD = 500;   // Threshold for black vs white detection

// Status tracking
bool thirdFieldActive = false;
int transitionCount = 0;
BackgroundType backgroundType = BACKGROUND_WHITE;

// External variables from main
extern const int IR_SENSOR_L_PIN;
extern const int IR_SENSOR_R_PIN; 
extern const int IR_SENSOR_S_PIN;
extern int blackLimit;
extern int baseSpeed;
extern float Kp, Ki, Kd;
extern bool whiteLineModeEnabled;
extern RobotField currentField;

// Initialize third field algorithm
void initThirdField() {
  thirdFieldActive = true;
  transitionCount = 0;
  
  // Detect the background color
  backgroundType = detectBackgroundColor();
  
  // Set white line mode flag based on background
  whiteLineModeEnabled = (backgroundType == BACKGROUND_BLACK);
  
  // Adjust PID parameters based on background color
  if (backgroundType == BACKGROUND_WHITE) {
    // Standard PID parameters for black line on white background
    Kp = 0.3;  // Standard proportional
    Ki = 0;    // Standard integral
    Kd = 5;    // Standard derivative
  } else {
    // Adjusted PID parameters for white line on black background
    Kp = 0.4;  // Increased proportional for better tracking of white line
    Ki = 0;    // Keep integral at 0
    Kd = 6;    // Increased derivative for smoother tracking
  }
  
  // Display status
  lcdclear();
  lcdShow(0, 0, "Third Field Mode");
  
  if (backgroundType == BACKGROUND_WHITE) {
    lcdShow(1, 0, "BG: White");
  } else {
    lcdShow(1, 0, "BG: Black");
  }
  
  delay(1000);
  lcdclear();
}

// Detect the background color by sampling the surface
BackgroundType detectBackgroundColor() {
  long colorSum = 0;
  
  // Take multiple samples to get a stable reading
  for (int i = 0; i < COLOR_SAMPLE_COUNT; i++) {
    colorSum += analogRead(IR_SENSOR_S_PIN);
    delay(10);
  }
  
  // Calculate average
  int colorAvg = colorSum / COLOR_SAMPLE_COUNT;
  
  // Determine background color based on threshold
  if (colorAvg > COLOR_DETECTION_THRESHOLD) {
    return BACKGROUND_WHITE;  // White background (high reflection)
  } else {
    return BACKGROUND_BLACK;  // Black background (low reflection)
  }
}

// Modified line following function for both background types
void lineFollowWithBackgroundType() {
  // Use the standard PID function
  // The getError() function in middleware.cpp will use the right error calculation 
  // based on the whiteLineModeEnabled flag
  pid(baseSpeed);
}

// Detect transition/cross based on background type
bool detectTransition() {
  // Use the standard isOnCross function which is now updated to handle both line types
  return isOnCross();
}

// Check if the third field is completed
bool isThirdFieldCompleted() {
  return transitionCount >= THIRD_FIELD_TRANSITION_COUNT;
}

// Main algorithm for the third field
void thirdFieldAlgorithm() {
  static unsigned long lastStatusTime = 0;
  static unsigned long lastTransitionDetectionTime = 0;
  static bool wasOnTransition = false;
  
  // If not in third field mode, initialize
  if (!thirdFieldActive) {
    initThirdField();
  }
  
  // Follow the line with modified PID
  lineFollowWithBackgroundType();
  
  // Display status periodically
  if (millis() - lastStatusTime > 1000) {
    lcdShow(0, 0, "Third Field");
    lcdShow(1, 0, "Trans: ");
    lcdShow(1, 7, transitionCount);
    lastStatusTime = millis();
  }
  
  // Check for transitions
  bool onTransition = detectTransition();
  
  // Debounce transition detection
  if (onTransition && !wasOnTransition && (millis() - lastTransitionDetectionTime > 500)) {
    transitionCount++;
    lastTransitionDetectionTime = millis();
    
    // Display transition detection
    lcdShow(0, 0, "Transition!");
    lcdShow(1, 0, "Count: ");
    lcdShow(1, 7, transitionCount);
    
    // Small delay to avoid multiple counts
    delay(100);
  }
  
  wasOnTransition = onTransition;
  
  // Check if we've completed the third field
  if (isThirdFieldCompleted()) {
    stop();
    lcdclear();
    lcdShow(0, 0, "Third field done");
    lcdShow(1, 0, "Transitions: ");
    lcdShow(1, 12, transitionCount);
    delay(2000);
    
    thirdFieldActive = false;
    
    // Transition to finish
    lcdclear();
    lcdShow(0, 0, "Third completed");
    lcdShow(1, 0, "Go to Finish");
    delay(1000);
    
    currentField = FIELD_FINISH;
    return;
  }
} 