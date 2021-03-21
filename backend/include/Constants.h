#pragma once
#define _USE_MATH_DEFINES

#include "Utils.h"

const int BACKEND_TO_FRONTEND_CALLS_PER_SECOND = 166;
const int ENCODER_CYCLE = 20;
const double X_DEADZONE = 0.15;
const double Y_DEADZONE = 0.15;
const double Z_DEADZONE = 0.15;
const double TARGET_MAGNITUDE_SCALAR = 50.0;
//It actually is the number of inches per revolution
const double intelCoreCount = 2.35619449;
const double inchesToClicksMultiplier = ENCODER_CYCLE / intelCoreCount;
const double clicksToInchesMultiplier = intelCoreCount / ENCODER_CYCLE;
const double width = 81;
const double height = 98.5;
const int CENTER_POSITION_IN_CLICKS = RoundLit(PythagC(width / 2.0, height / 2.0) * inchesToClicksMultiplier);

const double WINCH_1_X = 0.0;
const double WINCH_1_Y = height;
const double WINCH_2_X = width;
const double WINCH_2_Y = height;

const bool DEBUG = true;