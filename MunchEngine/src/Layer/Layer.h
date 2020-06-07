#pragma once

#include "Event/Event.h"
#include "Event/Types/MouseMoveEvent.h"
#include "Event/Types/MousePressEvent.h"
#include "Event/Types/KeyEvent.h"
#include "Event/Types/WindowResizeEvent.h"

#include <string>

// TODO: make this an abstract class

struct Layer {
    Layer(std::string name, int width, int height);
    virtual ~Layer();

    void onEvent(Event& event);
    virtual bool onMouseMove(MouseMoveEvent& event);
    virtual bool onMousePress(MousePressEvent& event);
    virtual bool onKeyPress(KeyPressEvent& event);
    virtual bool onKeyRelease(KeyReleaseEvent& event);
    virtual bool onKeyRepeat(KeyRepeatEvent& event);
    virtual bool onWindowResize(WindowResizeEvent& event);

    virtual void setup();
    virtual void update();

    std::string name;
    int width, height;
};