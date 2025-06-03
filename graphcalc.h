#ifndef GRAPHCALC_H
#define GRAPHCALC_H

#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/ioctl.h>
#endif

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_GRAY    "\033[90m"

// Function declarations
void clearScreen();
void enableAnsiOnWindows();
void getTerminalSize(int &width, int &height);
double evaluateFunction(const std::string &expr, double x);
void plotGraph(const std::string &expression, double xMin, double xMax, double yMin, double yMax);

#endif // GRAPHCALC_H
