#pragma once

#include <Arduino.h>

enum class ControllerMode : uint8_t {
    NORMAL,
    CALIBRATION_START,
    CALIBRATION_LIMITS,
    CALIBRATION_DEADZONE,
    CALIBRATION_FINISH
};