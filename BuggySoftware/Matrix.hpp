/*
 * @file matrix.hpp
 * @brief Setup methods for matrix
 */
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Arduino.h"
#include "Arduino_LED_Matrix.h"

// Enum for different buggy modes
enum class BuggyMode {
    LINE_FOLLOWING,
    STOPPED,
    OBSTACLE_DETECTED
};

class Matrix {
private:
    ArduinoLEDMatrix matrix;
    BuggyMode currentMode;

    void displayLineFollowing();
    void displayStopped();
    void displayObstacleDetected();

    static uint8_t arrowPattern[8][12];
    static uint8_t stopPattern[8][12];
    static uint8_t warningPattern[8][12];

public:
    Matrix();
    void begin();
    void setMode(BuggyMode mode);
};

#endif