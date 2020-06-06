#pragma once

#include "Event/Event.h"

#include <functional>

struct EventDispatcher {
    template<typename T>
    using Callable = std::function<bool(T&)>;

    EventDispatcher(Event& event);
    ~EventDispatcher();

    template<Event::Type type, typename Func>
    void dispatch(Func&& func);

    Event& event;
};

template<Event::Type type, typename Func>
inline void EventDispatcher::dispatch(Func&& func) {
    if(event.type == type && !event.handled) {
        event.handled = func(event.as<typename EventClass<type>::T>());
    }
}