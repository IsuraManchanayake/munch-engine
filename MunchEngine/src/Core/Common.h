#pragma once 

#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>

#undef near
#undef far

// Errors
void error_exit(int code);

// Console
void setConsoleColor(WORD* Attributes, DWORD Colour);
void resetConsoleColor(WORD Attributes);

// Files
std::string readFile(const std::string& path);

// Memory
template<typename T, size_t N> constexpr size_t size(const T(&array)[N]);

// String
template<typename T, typename ...Args> std::string cat(T&& first, Args&&... args);
void split(std::vector<std::string>& results, const std::string& input, char character);
template <typename T> T lexical_cast(const std::string& input);
template <typename T, typename ConstIter> T lexical_cast(ConstIter first, ConstIter last);
template<typename ToType> void split_cast(std::vector<ToType>& results, const std::string& input, char character);

// ================ Template definitions ================
template<typename T, typename ...Args>
inline std::string cat(T&& first, Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

template <typename T> 
inline T lexical_cast(const std::string& input) {
    uint64_t n = 0;
    bool neg = input[0] == '-';
    const char* s = input.c_str();
    for (size_t j = neg ? 1 : 0, ssz = input.size(); j < ssz; j++) {
        char c = s[j] - '0';
        n = n * 10 + c;
    }
#pragma warning(push)
#pragma warning(disable : 4146)
    return neg ? -static_cast<T>(n) : static_cast<T>(n);
#pragma warning(pop)
}

template <typename T, typename ConstIter>
inline T lexical_cast(ConstIter first, ConstIter last) {
    return lexical_cast<T>(std::string(first, last));
}

template<typename ToType>
inline void split_cast(std::vector<ToType>& results, const std::string& input, char character) {
    size_t previous = 0;
    for (size_t i = 0, sz = input.size(); i < sz; i++) {
        if (input[i] == character) {
            results.emplace_back(lexical_cast<ToType>(&input[previous], &input[i]));
            previous = i + 1;
        }
    }
    results.emplace_back(lexical_cast<ToType>(&input[previous], &input[input.size()]));
}

template<typename T, size_t N>
constexpr size_t size(const T(&array)[N]) {
    return N;
}