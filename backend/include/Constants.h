#pragma once
#define _USE_MATH_DEFINES

#include "Utils.h"

const int BACKEND_TO_FRONTEND_CALLS_PER_SECOND = 166;
const int ENCODER_CYCLE = 20;
const double X_DEADZONE = 0.15;
const double Y_DEADZONE = 0.15;
const double Z_DEADZONE = 0.15;
const double TARGET_MAGNITUDE_SCALAR = 1.0;
//It actually is the number of inches per revolution
const double INCHES_PER_ENCODER_REV = 2.35619449;
const double INCHES_TO_CLICKS_MULTIPLIER = ENCODER_CYCLE / INCHES_PER_ENCODER_REV;
const double CLICKS_TO_INCHES_MULTIPLIER = INCHES_PER_ENCODER_REV / ENCODER_CYCLE;
const double WIDTH_INCHES = 80.0;
const double HEIGHT_INCHES = 80.0;
const double WIDTH_FULL_RES = WIDTH_INCHES * INCHES_TO_CLICKS_MULTIPLIER;
const double HEIGHT_FULL_RES = HEIGHT_INCHES * INCHES_TO_CLICKS_MULTIPLIER;
const int CENTER_POSITION_IN_CLICKS = RoundLit(PythagC(WIDTH_FULL_RES / 2.0, HEIGHT_FULL_RES / 2.0));

const double WINCH_1_X = 0.0;
const double WINCH_1_Y = HEIGHT_FULL_RES;
const double WINCH_2_X = WIDTH_FULL_RES;
const double WINCH_2_Y = HEIGHT_FULL_RES;

const bool DEBUG = true;