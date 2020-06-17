#pragma once

#include "Core/Common.h"
#include "Config/Config.h"

#include <iostream>
#include <deque>
#include <Windows.h>
#include <thread>
#include <mutex>

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

enum class LogLevel {
    None = 0,
    Trace = 0x08,
    Debug = 0x0f,
    Info = 0x0a,
    Warn = 0x0e,
    Error = 0x0c
};

struct LogQueue {
    LogQueue();

    std::pair<LogLevel, std::string> pop();
    void enque(LogLevel level, std::string line);

    std::deque<std::pair<LogLevel, std::string>> queue;
    mutable std::mutex queueMutex;
};

struct Logger {
    static void init();
    static void stop();
    
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

    static WORD consoleAttribs;
    static LogQueue logQueue;
    static bool enabled;
    static std::thread loggerThread;
};

// ================ Template definitions ================
template<typename ...Args>
inline void Logger::trace(Args&&... args) {
#if LOG_LEVEL <= LOG_TRACE
    logger.enque(LogLevel::Trace, cat(currentTime(), " [TRACE] ", std::forward<Args>(args)...));
#endif
}

template<typename ...Args>
inline void Logger::debug(Args&&... args) {
#if LOG_LEVEL <= LOG_DEBUG
    logger.enque(LogLevel::Debug, cat(currentTime(), " [DEBUG] ", std::forward<Args>(args)...));
#endif
}

template<typename ...Args>
inline void Logger::info(Args&&... args) {
#if LOG_LEVEL <= LOG_INFO
    logQueue.enque(LogLevel::Info, cat(currentTime(), " [INFO]  ", std::forward<Args>(args)...));
#endif
}

template<typename ...Args>
inline void Logger::warn(Args&&... args) {
#if LOG_LEVEL <= LOG_WARN
    logQueue.enque(LogLevel::Warn, cat(currentTime(), " [WARN]  ", std::forward<Args>(args)...));
#endif
}

template<typename ...Args>
inline void Logger::error(Args&&... args) {
#if LOG_LEVEL <= LOG_ERROR
    logQueue.enque(LogLevel::Error, cat(currentTime(), " [ERROR] ", std::forward<Args>(args)...));
#endif
}
