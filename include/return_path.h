#ifndef RETURN_PATH_H
#define RETURN_PATH_H

// Initialize the return path algorithm
void initReturnPath();

// Main algorithm for the return path
void returnPathAlgorithm();

// Check if the return path is completed
bool isReturnPathCompleted();

// Functions for field navigation
void navigateThirdFieldReverse();
void navigateMazeReverse();
void navigateFirstFieldReverse();

// Functions for line following in reverse
void followLineReverse();
void handleCrossingsReverse();

#endif // RETURN_PATH_H 