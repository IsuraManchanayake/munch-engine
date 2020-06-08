#include "Layer/Layer.h"
#include "Event/EventDispatcher.h"
#include "Core/Logger.h"

#include <functional>

Layer::Layer(std::string name, int width, int height)
    : name(std::move(name)), width(width), height(height) {
}

Layer::~Layer() {
}

void Layer::onEvent(Event& event) {
    Logger::trace("Event in layer level \"", name, "\" ", event);
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<Event::Type::MouseMove>(std::bind(&Layer::onMouseMove, this, std::placeholders::_1));
    dispatcher.dispatch<Event::Type::MousePress>(std::bind(&Layer::onMousePress, this, std::placeholders::_1));
    dispatcher.dispatch<Event::Type::KeyPress>(std::bind(&Layer::onKeyPress, this, std::placeholders::_1));
    dispatcher.dispatch<Event::Type::KeyRelease>(std::bind(&Layer::onKeyRelease, this, std::placeholders::_1));
    dispatcher.dispatch<Event::Type::KeyRepeat>(std::bind(&Layer::onKeyRepeat, this, std::placeholders::_1));
    dispatcher.dispatch<Event::Type::WindowResize>(std::bind(&Layer::onWindowResize, this, std::placeholders::_1));
}

bool Layer::onMouseMove(MouseMoveEvent& event) {
    Logger::info(event);
    return true;
}

bool Layer::onMousePress(MousePressEvent& event) {
    Logger::info(event);
    return true;
}

bool Layer::onKeyPress(KeyPressEvent& event) {
    Logger::info(event);
    return true;
}

bool Layer::onKeyRelease(KeyReleaseEvent& event) {
    Logger::info(event);
    return true;
}

bool Layer::onKeyRepeat(KeyRepeatEvent& event) {
    Logger::info(event);
    return true;
}

bool Layer::onWindowResize(WindowResizeEvent& event) {
    width = event.width;
    height = event.height;
    return false; // Should propagate to all layers
}

void Layer::setup() {
}

void Layer::update() {
}
