#pragma once

#include <iostream>
#include <Windows.h>

#include "Common.h"

/**
 * 
 * Color chart
 * 
 * 0 = Black
 * 8 = Gray
 * 
 * 1 = Blue
 * 9 = Light Blue
 * 
 * 2 = Green
 * A = Light Green
 * 
 * 3 = Aqua
 * B = Light Aqua
 * 
 * 4 = Red
 * C = Light Red
 * 
 * 5 = Purple
 * D = Light Purple
 * 
 * 6 = Yellow
 * E = Light Yellow
 * 
 * 7 = White
 * F = Bright White
 * 
 * The first number denotes background color
 * The second number denotes foreground color
 * 
 */

struct Logger {
    static WORD consoleAttribs;

    template<typename ...Args>
    static void trace(Args&&... args);
    template<typename ...Args>
    static void debug(Args&&... args);
    template<typename ...Args>
    static void info(Args&&... args);
    template<typename ...Args>
    static void warn(Args&&... args);
    template<typename ...Args>
    static void error(Args&&... args);

    static std::string currentTime();
};

// ================ Template definitions ================
template<typename ...Args>
inline void Logger::trace(Args&&... args) {
    setConsoleColor(&consoleAttribs, 0x08);
    std::cout << currentTime() << " [TRACE] ";
    std::cout << cat(std::forward<Args>(args)...) << '\n';
    resetConsoleColor(consoleAttribs);
}

template<typename ...Args>
inline void Logger::debug(Args&&... args) {
    setConsoleColor(&consoleAttribs, 0x0f);
    std::cout << currentTime() << " [DEBUG] ";
    std::cout << cat(std::forward<Args>(args)...) << '\n';
    resetConsoleColor(consoleAttribs);
}

template<typename ...Args>
inline void Logger::info(Args&&... args) {
    setConsoleColor(&consoleAttribs, 0x0a);
    std::cout << currentTime() << " [INFO]  ";
    std::cout << cat(std::forward<Args>(args)...) << '\n';
    resetConsoleColor(consoleAttribs);
}

template<typename ...Args>
inline void Logger::warn(Args&&... args) {
    setConsoleColor(&consoleAttribs, 0x0e);
    std::cout << currentTime() << " [WARN]  ";
    std::cout << cat(std::forward<Args>(args)...) << '\n';
    resetConsoleColor(consoleAttribs);
}

template<typename ...Args>
inline void Logger::error(Args&&... args) {
    setConsoleColor(&consoleAttribs, 0x0c);
    std::cout << currentTime() << " [ERROR] ";
    std::cout << cat(std::forward<Args>(args)...) << '\n';
    resetConsoleColor(consoleAttribs);
}
