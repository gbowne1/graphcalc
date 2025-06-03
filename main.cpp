#include "graphcalc.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    enableAnsiOnWindows();
    std::string function;
    double xMin = -10.0, xMax = 10.0, yMin = -10.0, yMax = 10.0;

    while (true) {
        clearScreen();
        std::cout << "Terminal Graphing Calculator (Improved Version)\n";
        std::cout << "Supported: sin(x), cos(x), tan(x), log(x), x^2, x^3, x\n";
        std::cout << "Enter function to plot (or 'q' to quit): ";
        std::getline(std::cin, function);

        if (function == "q" || function == "quit") break;

        if (!(function == "sin(x)" || function == "cos(x)" || function == "tan(x)" ||
              function == "log(x)" || function == "x^2" || function == "x^3" || function == "x")) {
            std::cerr << "Unsupported function.\n";
            std::cin.get();
            continue;
        }

        std::cout << "Enter xMin, xMax, yMin, yMax: ";
        std::string rangeInput;
        std::getline(std::cin, rangeInput);
        std::stringstream ss(rangeInput);
        if (!(ss >> xMin >> xMax >> yMin >> yMax)) {
            std::cerr << "Invalid input. Please enter four numbers.\n";
            std::cin.get();
            continue;
        }

        plotGraph(function, xMin, xMax, yMin, yMax);
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    return 0;
}
