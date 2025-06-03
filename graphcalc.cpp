#include "graphcalc.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void enableAnsiOnWindows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
#endif
}

void getTerminalSize(int &width, int &height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
#endif
}

double evaluateFunction(const std::string &expr, double x) {
    if (expr == "sin(x)") return sin(x);
    if (expr == "cos(x)") return cos(x);
    if (expr == "tan(x)") {
        double c = cos(x);
        if (std::fabs(c) < 1e-6) return std::numeric_limits<double>::quiet_NaN();
        return tan(x);
    }
    if (expr == "log(x)") return (x > 0) ? log(x) : std::numeric_limits<double>::quiet_NaN();
    if (expr == "x^3") return x * x * x;
    if (expr == "x^2") return x * x;
    if (expr == "x") return x;
    return 0.0;
}

void plotGraph(const std::string &expression, double xMin, double xMax, double yMin, double yMax) {
    int termWidth = 0, termHeight = 0;
    getTerminalSize(termWidth, termHeight);

    if (termWidth < 20 || termHeight < 10) {
        std::cerr << "Terminal too small to plot the graph.\n";
        return;
    }

    int graphWidth = termWidth - 8;
    int graphHeight = termHeight - 6;

    if (graphWidth <= 0 || graphHeight <= 0) {
        std::cerr << "Graph dimensions invalid after margins.\n";
        return;
    }

    if (xMax == xMin || yMax == yMin) {
        std::cerr << "Invalid graph range (division by zero risk).\n";
        return;
    }

    std::vector<std::vector<char>> graph(graphHeight, std::vector<char>(graphWidth, ' '));

    if (expression == "tan(x)") {
        for (int j = 0; j < graphWidth; ++j) {
            double x = xMin + (xMax - xMin) * j / graphWidth;
            if (std::fabs(std::cos(x)) < 1e-6) {
                for (int i = 0; i < graphHeight; ++i)
                    graph[i][j] = ':';
            }
        }
    }

    for (int i = 0; i < graphHeight; ++i) {
        double y = yMax - (yMax - yMin) * i / graphHeight;
        if (std::fmod(y, 5.0) < 0.1) {
            for (int j = 0; j < graphWidth; ++j)
                if (graph[i][j] == ' ') graph[i][j] = '.';
        }
    }

    for (int j = 0; j < graphWidth; ++j) {
        double x = xMin + (xMax - xMin) * j / graphWidth;
        double y = evaluateFunction(expression, x);
        if (std::isnan(y) || std::isinf(y)) continue;
        int yPos = static_cast<int>((y - yMin) / (yMax - yMin) * graphHeight);
        if (yPos >= 0 && yPos < graphHeight)
            graph[graphHeight - yPos - 1][j] = '*';
    }

    clearScreen();
    std::cout << "Graphing: " << expression << "\n\n";

    for (int i = 0; i < graphHeight; ++i) {
        double yLabel = yMin + (yMax - yMin) * (graphHeight - i - 1) / graphHeight;
        std::cout << std::setw(6) << std::fixed << std::setprecision(1) << yLabel << " |";

        for (int j = 0; j < graphWidth; ++j) {
            char ch = graph[i][j];
            if (ch == '*') std::cout << COLOR_GREEN << '*' << COLOR_RESET;
            else if (ch == '.') std::cout << COLOR_GRAY << '.' << COLOR_RESET;
            else if (ch == ':') std::cout << COLOR_GRAY << '|' << COLOR_RESET;
            else std::cout << ch;
        }
        std::cout << std::endl;
    }
}
