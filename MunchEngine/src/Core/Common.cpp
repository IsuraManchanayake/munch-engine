#include "Common.h"

#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

void error_exit(int code) {
    exit(code);
}

void setConsoleColor(WORD* Attributes, DWORD Colour) {
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    *Attributes = Info.wAttributes;
    SetConsoleTextAttribute(hStdout, Colour);
}

void resetConsoleColor(WORD Attributes) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

std::string readFile(const std::string& path) {
    std::ifstream ifs(path);
    return { (std::istreambuf_iterator<char>(ifs)),
                 std::istreambuf_iterator<char>() };
}

void split(std::vector<std::string>& results, const std::string& input,
    char character) {
    size_t previous = 0;
    for (size_t i = 0, sz = input.size(); i < sz; i++) {
        if (input[i] == character) {
            results.emplace_back(&input[previous], &input[i]);
            previous = i + 1;
        }
    }
    results.emplace_back(&input[previous], &input[input.size()]);
}

std::string currentTime() {
    std::time_t const now_c = std::time(nullptr);
    return cat(std::put_time(std::localtime(&now_c), "%F %T"));
}