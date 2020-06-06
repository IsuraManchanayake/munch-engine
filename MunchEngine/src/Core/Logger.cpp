#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>

WORD Logger::consoleAttribs = 0;

std::string Logger::currentTime() {
    std::time_t const now_c = std::time(nullptr);
    return cat(std::put_time(std::localtime(&now_c), "%F %T"));
}