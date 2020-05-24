#pragma once

#include "Utils.h"

const int BACKEND_TO_FRONTEND_CALLS_PER_SECOND = 166;

const int ENCODER_CYCLE = 20;

//It actually is the number of inches per revolution
const double intelCoreCount = 2.35619449;
const double inchesToClicksMultiplier = ENCODER_CYCLE / intelCoreCount;
const double clicksToInchesMultiplier = intelCoreCount / ENCODER_CYCLE;
const int width = 87;
const int height = 80;
const int centerPositionInClicks = RoundLit(PythagC(width / 2.0, height / 2.0) * inchesToClicksMultiplier);