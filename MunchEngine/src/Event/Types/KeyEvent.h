#pragma once

#include "Event/Event.h"

#include <array>

using KeyContainer = std::array<uint16_t, 1024>;

struct KeyEvent : Event {
    KeyEvent(Event::Type type, const KeyContainer& keys, int key, int action);

    const KeyContainer& keys;
    int key, action;
};

struct KeyPressEvent : KeyEvent {
    KeyPressEvent(const KeyContainer& keys, int key);
};

struct KeyReleaseEvent : KeyEvent {
    KeyReleaseEvent(const KeyContainer& keys, int key);
};

struct KeyRepeatEvent : KeyEvent {
    KeyRepeatEvent(const KeyContainer& keys, int action);
};

template<>
struct EventClass<Event::Type::KeyPress> {
    using T = KeyPressEvent;
};

template<>
struct EventClass<Event::Type::KeyRelease> {
    using T = KeyReleaseEvent;
};

template<>
struct EventClass<Event::Type::KeyRepeat> {
    using T = KeyRepeatEvent;
};

inline std::ostream& operator<<(std::ostream& os, const KeyEvent& event) {
    os << "{[KeyEvent]";
    switch(event.type) {
        case Event::Type::KeyPress: {
            os << " KeyPress";
            break;
        }
        case Event::Type::KeyRelease: {
            os << " KeyRelease";
            break;
        }
        case Event::Type::KeyRepeat: {
            os << " KeyRepeat";
            break;
        }
    }
    os << " key:" << event.key << " action:" << event.action;
    return os << "}";
}