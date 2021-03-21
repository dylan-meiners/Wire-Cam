#pragma once

#include <vector>
#include <Windows.h>

double PythagC(const double a, const double b);
int RoundLit(double x);
double Radians(double x);
double Degrees(double x);
double sq(double x);
unsigned char StandardToUCharRange(const double x);

std::vector<std::vector<double>> rref(std::vector<std::vector<double>>& x);
void PrintVector(std::vector<double>& x);
void PrintVector(std::vector<std::vector<double>>& x);
double StandardDegrees(const double i, const double j);

bool GetKeyDown(int key);