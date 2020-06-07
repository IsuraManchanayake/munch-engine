#include "Event/Types/MouseMoveEvent.h"

MouseMoveEvent::MouseMoveEvent(float x, float y, float dx, float dy) 
    : Event(Event::Type::MouseMove), x(x), y(y), dx(dx), dy(dy) {
}