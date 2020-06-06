#pragma once 

#include <string>
#include <sstream>
#include <Windows.h>

void error_exit(int code);
template<typename T, typename ...Args>
std::string cat(T&& first, Args&&... args);
void setConsoleColor(WORD* Attributes, DWORD Colour);
void resetConsoleColor(WORD Attributes);

// ================ Template definitions ================

inline std::string cat() {
    return {};
}

template<typename T, typename ...Args>
inline std::string cat(T&& first, Args&&... args) {
    std::ostringstream oss;
    oss << first << cat(std::forward<Args>(args)...);
    return oss.str();
}
