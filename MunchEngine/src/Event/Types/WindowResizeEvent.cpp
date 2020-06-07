#include "Event/Types/WindowResizeEvent.h"

WindowResizeEvent::WindowResizeEvent(int width, int height)
    : Event(Event::Type::WindowResize), width(width), height(height) {
}
