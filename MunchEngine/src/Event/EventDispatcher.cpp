#include "Event/EventDispatcher.h"

EventDispatcher::EventDispatcher(Event& event) 
    : event(event) {
}

EventDispatcher::~EventDispatcher() {
}