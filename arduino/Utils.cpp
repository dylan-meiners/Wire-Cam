#include "Utils.h"
#include <math.h>

double PythagC(const double a, const double b) { return sqrt(sq(a) + sq(b)); }
int RoundLit(long double x) {
    
    if (x >= 0) { return (x - (int)x >= .5) ? (int)x + 1 : (int)x; }
    else { return -((-x - (int)-x >= .5) ? (int)-x + 1 : (int)-x); }
}
double MapUCharToMicroseconds(const double value) { return (value / 255.0) * 1000 + 1000; }