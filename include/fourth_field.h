#ifndef FOURTH_FIELD_H
#define FOURTH_FIELD_H

// Constants for the fourth field
const int NON_CENTRAL_THRESHOLD = 20;  // Distance threshold for non-central cans (cm)
const int CENTRAL_THRESHOLD = 40;      // Distance threshold for central can (cm)
const int TARGET_ENCODER_COUNT = 1000; // Target encoder count for circle completion

// Initialize the fourth field algorithm
void initFourthField();

// Main algorithm for the fourth field
void fourthFieldAlgorithm();

// Check if the fourth field is completed
bool isFourthFieldCompleted();

// Functions for can processing
void processNonCentralCan();
void processCentralCan();

// Functions for circle navigation
void enterCircle();
void exitCircle();
void followCircleLine();

// Functions for encoder management
void resetEncoderAccumulation();
void updateEncoderAccumulation();
bool isEncoderTargetReached();

// Helper functions
void processCan();
void navigateCircle();
bool transitionToCircle();
bool isCircleDetected();
bool isCanDetected();

#endif // FOURTH_FIELD_H 