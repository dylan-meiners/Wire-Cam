#include <iostream>

double MapUCharToMicroseconds(const double value) {

    return (value / 255) * 1000 - 500;
}

int main() {

    std::cout << MapUCharToMicroseconds(255) << std::endl;

    return 0;
}