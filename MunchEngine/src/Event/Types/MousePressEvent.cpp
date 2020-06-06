#include "Event/Types/MousePressEvent.h"

MousePressEvent::MousePressEvent(int button, int action, int mods)
    : Event(Event::Type::MousePress), button(button), action(action), mods(mods) {
}

MousePressEvent::~MousePressEvent() {
}
