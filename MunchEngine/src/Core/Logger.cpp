#include "Core/Logger.h"


LogQueue::LogQueue()
    : queue(), queueMutex() {
}

void LogQueue::enque(LogLevel level, std::string line) {
    std::lock_guard<std::mutex> guard(queueMutex);
    queue.emplace_back(level, line);
}

std::pair<LogLevel, std::string> LogQueue::pop() {
    std::lock_guard<std::mutex> guard(queueMutex);
    auto p = queue.front();
    queue.pop_front();
    return p;
}

WORD Logger::consoleAttribs = 0;
LogQueue Logger::logQueue;
bool Logger::enabled = false;
std::thread Logger::loggerThread;

void Logger::init() {
    enabled = true;
    loggerThread = std::thread([]() {
        while(Logger::enabled) {
            if(Logger::logQueue.queue.size() > 0) {
                auto p = Logger::logQueue.pop();
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