#include "fourth_field.h"
#include "header.h"
#include "drive.h"
#include "encoder.h"
#include "UZD.h"
#include "servoMotor.h"
#include <Arduino.h>
#include <middleware.h>

// Constants
const int NON_CENTRAL_CAN_THRESHOLD = 20;  // cm
const int CENTRAL_CAN_THRESHOLD = 15;      // cm
const int CIRCLE_TARGET_ENCODER_COUNT = 1000;     // Adjust based on circle size
const int APPROACH_SPEED = 150;            // Speed for approaching cans
const int CIRCLE_SPEED = 170;              // Speed for circle navigation

// State variables
static bool isCompleted = false;
static int accumulatedTicks = 0;
static bool inProcessingState = false;

enum CircleState {
    FINDING_CIRCLE,
    NAVIGATING_CIRCLE,
    PROCESSING_CAN,
    EXITING_CIRCLE
} currentState;

void initFourthField() {
    isCompleted = false;
    accumulatedTicks = 0;
    inProcessingState = false;
    currentState = FINDING_CIRCLE;
    closeServo(); // Ensure servo is in closed position at start
}

void fourthFieldAlgorithm() {
    switch (currentState) {
        case FINDING_CIRCLE:
            if (transitionToCircle()) {
                currentState = NAVIGATING_CIRCLE;
            }
            break;

        case NAVIGATING_CIRCLE:
            if (isCanDetected()) {
                currentState = PROCESSING_CAN;
                inProcessingState = true;
                break;
            }
            
            navigateCircle();
            
            if (accumulatedTicks >= CIRCLE_TARGET_ENCODER_COUNT) {
                currentState = EXITING_CIRCLE;
            }
            break;

        case PROCESSING_CAN:
            processCan();
            if (!inProcessingState) {
                currentState = NAVIGATING_CIRCLE;
            }
            break;

        case EXITING_CIRCLE:
            exitCircle();
            isCompleted = true;
            break;
    }
}

bool isFourthFieldCompleted() {
    return isCompleted;
}

void processCan() {
    static int processingStage = 0;
    float frontDistance = getFDistance();
    
    switch (processingStage) {
        case 0: // Initial approach
            if (frontDistance <= CENTRAL_CAN_THRESHOLD) {
                stop();
                processingStage = 1;
                delay(200);
            } else {
                // Slowly approach the can
                go(APPROACH_SPEED, APPROACH_SPEED);
            }
            break;

        case 1: // Prepare to grab
            openServo();
            delay(500);
            processingStage = 2;
            break;

        case 2: // Move forward to capture
            go(APPROACH_SPEED/2, APPROACH_SPEED/2);
            delay(500);
            closeServo();
            delay(500);
            processingStage = 3;
            break;

        case 3: // Back up with can
            driveDirect = true;
            while(getIRSensorValue(IR_SENSOR_L_PIN) > blackLimit && getIRSensorValue(IR_SENSOR_R_PIN) > blackLimit) {
                pid(-baseSpeed);
            }
            driveDirect = false;
            doezd();
            left();
            stop();
            processingStage = 4;
            processingStage = 0;
            inProcessingState = false;
            break;

    }
}

void navigateCircle() {
    // Follow the circular line using PID control
    pid(CIRCLE_SPEED);
    
    // Update encoder accumulation
    accumulatedTicks += getEncoderError();
}

bool transitionToCircle() {
    static int transitionStage = 0;
    static unsigned long transitionTimer = 0;
    
    switch (transitionStage) {
        case 0: // Initial approach
            if (isCircleDetected()) {
                stop();
                transitionStage = 1;
                transitionTimer = millis();
                return false;
            }
            pid(baseSpeed); // Use existing line following
            return false;

        case 1: // Align with circle
            if (millis() - transitionTimer > 500) {
                transitionStage = 2;
                return true;
            }
            go(CIRCLE_SPEED, CIRCLE_SPEED);
            return false;

        default:
            return true;
    }
}

void exitCircle() {
    // Stop motors
    stop();
    delay(500);
    
    // Move forward slightly to clear the circle
    go(CIRCLE_SPEED, CIRCLE_SPEED);
    delay(1000);
    
    // Turn to align with exit path
    go(-CIRCLE_SPEED, CIRCLE_SPEED);
    delay(povorotDelay/2); // Using your existing constant
    
    stop();
}

bool isCircleDetected() {
    // Read all three IR sensors
    int leftValue = getIRSensorValue(IR_SENSOR_L_PIN);
    int rightValue = getIRSensorValue(IR_SENSOR_R_PIN);
    int centerValue = getIRSensorValue(IR_SENSOR_S_PIN);
    
    // Circle is detected when all sensors see the line
    return (leftValue < blackLimit && 
            rightValue < blackLimit && 
            centerValue < blackLimit);
}

bool isCanDetected() {
    float distance = getFDistance();
    return (distance > 0 && distance <= NON_CENTRAL_CAN_THRESHOLD);
}

bool isLineDetected() {
    int centerValue = getIRSensorValue(IR_SENSOR_S_PIN);
    return centerValue < blackLimit;
} 