#include "Event/Types/KeyEvent.h"

KeyEvent::KeyEvent(Event::Type type, const KeyContainer& keys) 
    : Event(type), keys(keys) {
}

KeyPressEvent::KeyPressEvent(const KeyContainer& keys)
    : KeyEvent(Event::Type::KeyPress, keys) {
}

KeyReleaseEvent::KeyReleaseEvent(const KeyContainer& keys)
    : KeyEvent(Event::Type::KeyRelease, keys) {
}

KeyRepeatEvent::KeyRepeatEvent(const KeyContainer& keys)
    : KeyEvent(Event::Type::KeyRepeat, keys) {
}