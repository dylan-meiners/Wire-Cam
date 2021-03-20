#pragma once

#define _USE_MATH_DEFINES

#include "../include/Utils.h"
#include <math.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <string>

double PythagC(const double a, const double b) { return sqrt(sq(a) + sq(b)); }
int RoundLit(double x) {
    
    if (x >= 0) { return (x - (int)x >= .5) ? (int)x + 1 : (int)x; }
    else { return -((-x - (int)-x >= .5) ? (int)-x + 1 : (int)-x); }
}
double Radians(double x) { return M_PI / 180 * x; }
double Degrees(double x) { return 180 / M_PI * x; }
double sq(double x) { return x * x; }

void PrintVector(std::vector<double>& x) {

    std::cout << "[ ";
    for (int i = 0; i < x.size(); i++) {

        std::cout << std::to_string(x[i]) << " ";
    }
    std::cout << "]" << std::endl;
}

void PrintVector(std::vector<std::vector<double>>& x) {

    for (int i = 0; i < x.size(); i++) {

        PrintVector(x[i]);
    }
}

void Multiply(std::vector<double>& x, const double y) {

    for (int i = 0; i < x.size(); i++) {

        x[i] *= y;
    }
}

void Add(std::vector<double>& x, const double y) {

    for (int i = 0; i < x.size(); i++) {

        x[i] += y;
    }
}

void Add(std::vector<double>& x, std::vector<double>& y) {

    for (int i = 0; i < x.size(); i++) {

        x[i] += y[i];
    }
}

std::vector<std::vector<double>> rref(std::vector<std::vector<double>> &x) {

    const int ROWS = x.size();
    const int COLS = x[0].size();
    std::vector<std::vector<double>> result;
    // The number of rows in the identity matrix is equal to the number of cols
    // and the number of cols in x must be greater than or equal to the number
    // of rows in x
    for (int i = 0; i < ROWS; i++) {

        //PrintVector(x);
        //std::cout << std::endl;

        // Push back an empty vector of doubles (new row)
        result.push_back(std::vector<double>());

        // If the pivot of the current row is zero, switch it out with a row
        // that has a non-zero number where that pivot needs to be
        if (x[i][i] == 0) {

            // Search for the new row, starting with the row currently after
            // the one with the bad pivot
            bool searching = true;
            for (int j = i + 1; j < ROWS && searching; j++) {

                // If a valid row is found
                if (x[j][i] != 0) {

                    // Create a temporary buffer for saving the bad row's data
                    std::vector<double> temp;
                    for (int k = 0; k < COLS; k++) {

                        temp.push_back(x[i][k]);
                    }

                    // Replace the bad row's data with the valid row's data
                    for (int k = 0; k < COLS; k++) {

                        x[i][k] = x[j][k];
                    }

                    // Replace the old valid data's row with the bad row's data
                    for (int k = 0; k < COLS; k++) {

                        x[j][k] = temp[k];
                    }

                    // Write the valid row data to the result vector
                    for (int k = 0; k < COLS; k++) {

                        result[i].push_back(x[i][k]);
                    }
                    searching = false;
                }
            }
        }
        // Otherwise, that row can stay
        else {

            for (int j = 0; j < COLS; j++) {

                result[i].push_back(x[i][j]);
            }
        }
    }

    /*std::cout << "After row swaps" << std::endl;
    PrintVector(result);
    std::cout << std::endl;*/

    for (int i = 1; i < ROWS; i++) {

        for (int j = 0; j < COLS && j < i; j++) {

            if (result[i][j] != 0) {

                Multiply(result[i], -result[j][j] / result[i][j]);
                Add(result[i], result[j]);
            }
            /*std::cout << "After i = " << i << " and j = " << j << std::endl;
            PrintVector(result);
            std::cout << std::endl;*/
        }
    }

    /*std::cout << "After ref" << std::endl;
    PrintVector(result);
    std::cout << std::endl;*/

    for (int i = ROWS - 2; i >= 0; i--) {

        for (int j = ROWS - 1; j > 0; j--) {

            if (result[i][j] != 0) {

                Multiply(result[i], -result[j][j] / result[i][j]);
                Add(result[i], result[j]);
            }
            /*std::cout << "After i = " << i << " and j = " << j << std::endl;
            PrintVector(result);
            std::cout << std::endl;*/
        }
    }

    /*std::cout << "After upper ref" << std::endl;
    PrintVector(result);
    std::cout << std::endl;*/

    for (int i = 0; i < ROWS; i++) {

        Multiply(result[i], 1 / result[i][i]);
        /*std::cout << "After i = " << i << " and j = " << i << std::endl;
        PrintVector(result);
        std::cout << std::endl;*/
    }

    return result;
}