#define _USE_MATH_DEFINES

#include "../include/Utils.h"
#include <math.h>

double PythagC(const double a, const double b) { return sqrt(sq(a) + sq(b)); }
int RoundLit(double x) {
    
    if (x >= 0) { return (x - (int)x >= .5) ? (int)x + 1 : (int)x; }
    else { return -((-x - (int)-x >= .5) ? (int)-x + 1 : (int)-x); }
}
double Radians(double x) { return M_PI / 180 * x; }
double Degrees(double x) { return 180 / M_PI * x; }
double sq(double x) { return x * x; }