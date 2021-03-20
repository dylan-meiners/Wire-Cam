#pragma once

#include <vector>

double PythagC(const double a, const double b);
int RoundLit(double x);
double Radians(double x);
double Degrees(double x);
double sq(double x);

std::vector<std::vector<double>> rref(std::vector<std::vector<double>>& x);
void PrintVector(std::vector<double>& x);
void PrintVector(std::vector<std::vector<double>>& x);