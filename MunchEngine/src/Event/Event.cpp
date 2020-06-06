#include "Event/Event.h"


Event::Event(Event::Type type)
    : type(type), handled(false) {
}

Event::Event()
    : Event(Event::Type::None) {
}

Event::~Event() {
}