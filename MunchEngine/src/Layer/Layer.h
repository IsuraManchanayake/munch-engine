#pragma once

#include "Event/Event.h"
#include "Event/Types/MouseMoveEvent.h"
#include "Event/Types/MousePressEvent.h"
#include "Event/Types/KeyEvent.h"

#include <string>

struct Layer {
    Layer(std::string name);
    virtual ~Layer();

    void onEvent(Event& event);
    virtual bool onMouseMove(MouseMoveEvent& event);
    virtual bool onMousePress(MousePressEvent& event);
    virtual bool onKeyPress(KeyPressEvent& event);
    virtual bool onKeyRelease(KeyReleaseEvent& event);
    virtual bool onKeyRepeat(KeyRepeatEvent& event);

    std::string name;
};