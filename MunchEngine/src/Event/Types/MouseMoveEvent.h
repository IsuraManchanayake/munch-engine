#pragma once

#include "Event/Event.h"
#include "Core/Common.h"

#include <iostream>

struct MouseMoveEvent : Event {
    MouseMoveEvent(float x, float y, float dx, float dy);
    ~MouseMoveEvent();

    float x;
    float y;
    float dx;
    float dy;
};

template<>
struct EventClass<Event::Type::MouseMove> {
    using T = MouseMoveEvent;
};

inline std::ostream& operator<<(std::ostream& os, const MouseMoveEvent& event) {
    return os << cat("{[MouseMoveEvent] x:", event.x, " y:", event.y, " dx:", event.dx, " dy:", event.dy, "}");
}