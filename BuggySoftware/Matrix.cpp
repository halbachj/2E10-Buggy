// Matrix.cpp
#include "Matrix.hpp"

// Define bitmap patterns using 8-bit integers (correct for Arduino_LED_Matrix)
uint8_t Matrix::arrowPattern[8][12] = {
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
};

uint8_t Matrix::stopPattern[8][12] = {
    {0,0,0,0,1,1,1,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0},
    {0,0,1,0,1,0,0,0,0,1,0,0},
    {0,0,1,0,0,1,0,0,0,1,0,0},
    {0,0,1,0,0,0,1,0,0,1,0,0},
    {0,0,1,0,0,0,0,1,0,1,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0,0}
};

uint8_t Matrix::warningPattern[8][12] = {
    {0,1,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,1,0,0,0,0,1,1,0,0},
    {0,0,0,0,1,0,0,1,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,0,0,0,0},
    {0,0,0,1,0,0,0,0,1,0,0,0}
};

// Constructor
Matrix::Matrix() : currentMode(BuggyMode::STOPPED) {}

// Initialize the LED matrix
void Matrix::begin() {
    matrix.begin();
    setMode(BuggyMode::STOPPED);
}

// Update matrix based on mode
void Matrix::setMode(BuggyMode mode) {
    currentMode = mode;
    matrix.clear();
    switch (mode) {
        case BuggyMode::LINE_FOLLOWING:
            displayLineFollowing();
            break;
        case BuggyMode::STOPPED:
            displayStopped();
            break;
        case BuggyMode::OBSTACLE_DETECTED:
            displayObstacleDetected();
            break;
    }
}

// Define display functions with correct 8-bit data
void Matrix::displayLineFollowing() {
    matrix.renderBitmap(arrowPattern, 8, 12);
}

void Matrix::displayStopped() {
    matrix.renderBitmap(stopPattern, 8, 12);
}

void Matrix::displayObstacleDetected() {
    matrix.renderBitmap(warningPattern, 8, 12);
}
