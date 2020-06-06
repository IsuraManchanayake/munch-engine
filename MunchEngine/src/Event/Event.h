#pragma once

#include <iostream>

struct Event {
    enum class Type {
        None = 0,
        MouseMove, MousePress,
        KeyPress, KeyRelease, KeyRepeat 
    };

    Event();
    Event(Type type);
    virtual ~Event();

    template<typename DerivedEventType>
    DerivedEventType& as();

    Type type;
    bool handled;
};

template<Event::Type type>
struct EventClass;

template<typename DerivedEventType>
inline DerivedEventType& Event::as() {
    return *((DerivedEventType*)this);
}

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return os << "{[Event]}";
}