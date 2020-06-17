#pragma once

#include <string>

struct Screenshot {
    static void capturepng(std::string format, unsigned width, unsigned height);
};