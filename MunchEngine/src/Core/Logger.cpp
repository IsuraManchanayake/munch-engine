#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>

LoggerImpl::LoggerImpl()
    : queue() {
}

void LoggerImpl::enque(LogLevel level, std::string line) {
    queue.emplace_back(level, line);
}

std::pair<LogLevel, std::string> LoggerImpl::pop() {
    auto p = queue.front();
    queue.pop_front();
    return p;
}

WORD Logger::consoleAttribs = 0;
LoggerImpl Logger::logger;
bool Logger::enabled = false;
std::thread Logger::loggerThread;

void Logger::init() {
    enabled = true;
    loggerThread = std::thread([]() {
        while(Logger::enabled) {
            if(Logger::logger.queue.size() > 0) {
                auto p = Logger::logger.pop();
                unsigned attrib = static_cast<unsigned>(p.first);
                std::string& line = p.second;
                setConsoleColor(&consoleAttribs, attrib); 
                std::cout << line << '\n';
                resetConsoleColor(consoleAttribs); 
            }
        }
    });
}

void Logger::stop() {
    enabled = false;
    loggerThread.join();
}

std::string Logger::currentTime() {
    std::time_t const now_c = std::time(nullptr);
    return cat(std::put_time(std::localtime(&now_c), "%F %T"));
}