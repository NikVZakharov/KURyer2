#ifndef THIRD_FIELD_H
#define THIRD_FIELD_H

// Field background type
enum BackgroundType {
  BACKGROUND_WHITE = 0,  // White background, black line
  BACKGROUND_BLACK = 1   // Black background, white line
};

// Initialize the third field algorithm
void initThirdField();

// Main algorithm for the third field
void thirdFieldAlgorithm();

// Check if the third field is completed
bool isThirdFieldCompleted();

// Detect the background color of the field
BackgroundType detectBackgroundColor();

// Functions for line following on white/black backgrounds
int getIRErrorReversed();
bool isOnWhite(int sensor);
bool isOnCrossReversed();

#endif // THIRD_FIELD_H
