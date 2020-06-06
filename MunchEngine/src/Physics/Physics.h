#pragma once

#include <chrono>

class Physics {
private:
    static size_t startTime_l;
    static size_t prevTime_l;
    static size_t current_l;

    static size_t currentTImeMicroSeconds() {
        return current_l = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count();
    }

public:
    static float currentTimeSeconds;
    static float dt;
    static size_t frames;

    static void init() {
        startTime_l = currentTImeMicroSeconds();
        prevTime_l = startTime_l;
        frames = 0;
    }

    static void update() {
        currentTimeSeconds = 1e-6f * (currentTImeMicroSeconds() - startTime_l);
        dt = 1e-6f * (current_l - prevTime_l);
        prevTime_l = current_l;
        frames++;
    }
};
