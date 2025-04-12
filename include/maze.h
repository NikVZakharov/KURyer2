#ifndef MAZE_H
#define MAZE_H

// Initialize the maze navigation
void initMaze();

// Main maze navigation algorithm
void mazeAlgorithm();

// Check if robot has reached the end of the maze
bool isMazeCompleted();

// Function to perform one step of maze navigation
void mazeStep();

// Helper functions
void moveForward();
void turnRight();
void turnLeft();
void turnAround();

// Sensing functions
bool hasWallFront();
bool hasWallRight(); 
bool hasWallLeft();

// Toggle display mode between sensors and status
void toggleDisplayMode();

#endif // MAZE_H