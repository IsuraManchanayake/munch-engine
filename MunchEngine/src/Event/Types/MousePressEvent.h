#pragma once

#include "Event/Event.h"
#include "Core/Common.h"

struct MousePressEvent : Event {
    MousePressEvent(int button, int action, int mods);

    int button;
    int action;
    int mods;
};

template<>
struct EventClass<Event::Type::MousePress> {
    using T = MousePressEvent;
};

inline std::ostream& operator<<(std::ostream& os, const MousePressEvent& event) {
    return os << cat("{[MousePressEvent] button:", event.button, " action:", event.action, " mods: ", event.mods, "}");
}