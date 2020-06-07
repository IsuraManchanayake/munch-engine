#pragma once

#include "Event/Event.h"
#include "Core/Common.h"

struct WindowResizeEvent : Event {
    WindowResizeEvent(int width, int height);
    
    int width;
    int height;
};

template<>
struct EventClass<Event::Type::WindowResize> {
    using T = WindowResizeEvent;
};

inline std::ostream& operator<<(std::ostream& os, const WindowResizeEvent& event) {
    return os << cat("{[WindowResizeEvent] width:", event.width, " height:", event.height, "}");
}
