#include "Layer/TestLayer.h"

#include "Core/Logger.h"

TestLayer::TestLayer(std::string name, int width, int height) 
    : Layer(std::move(name), width, height) {
}

bool TestLayer::onMouseMove(MouseMoveEvent& event) {
    Logger::warn("TestLayer!");
    Logger::info(event);
    return true;
}

bool TestLayer::onMousePress(MousePressEvent& event) {
    Logger::warn("TestLayer!");
    Logger::info(event);
    return true;
}

bool TestLayer::onKeyPress(KeyPressEvent& event) {
    Logger::warn("TestLayer!");
    Logger::info(event);
    return false;
}

bool TestLayer::onKeyRelease(KeyReleaseEvent& event) {
    Logger::warn("TestLayer!");
    Logger::info(event);
    return true;
}

bool TestLayer::onKeyRepeat(KeyRepeatEvent& event) {
    Logger::warn("TestLayer!");
    Logger::info(event);
    return true;
}
