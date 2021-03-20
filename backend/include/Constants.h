#pragma once

#include "Utils.h"

const int BACKEND_TO_FRONTEND_CALLS_PER_SECOND = 166;
const int ENCODER_CYCLE = 20;
const double X_DEADZONE = 0.15;
const double Y_DEADZONE = 0.15;
const double Z_DEADZONE = 0.15;
//It actually is the number of inches per revolution
const double intelCoreCount = 2.35619449;
const double inchesToClicksMultiplier = ENCODER_CYCLE / intelCoreCount;
const double clicksToInchesMultiplier = intelCoreCount / ENCODER_CYCLE;
const double width = 81;
const double height = 98.5;
const int CENTER_POSITION_IN_CLICKS = RoundLit(PythagC(width / 2.0, height / 2.0) * inchesToClicksMultiplier);