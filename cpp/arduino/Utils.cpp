#include "Utils.h"
#include <math.h>

double PythagC(const double a, const double b) { return sqrt(sq(a) + sq(b)); }
int RoundLit(double x) {
    
    if (x >= 0) { return (x - (int)x >= .5) ? (int)x + 1 : (int)x; }
    else { return -((-x - (int)-x >= .5) ? (int)-x + 1 : (int)-x); }
}