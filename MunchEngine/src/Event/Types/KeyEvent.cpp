#include "Event/Types/KeyEvent.h"

#include <GLFW/glfw3.h>

KeyEvent::KeyEvent(Event::Type type, const KeyContainer& keys, int key, int action) 
    : Event(type), keys(keys), key(key), action(action) {
}

KeyPressEvent::KeyPressEvent(const KeyContainer& keys, int key)
    : KeyEvent(Event::Type::KeyPress, keys, key, GLFW_PRESS) {
}

KeyReleaseEvent::KeyReleaseEvent(const KeyContainer& keys, int key)
    : KeyEvent(Event::Type::KeyRelease, keys, key, GLFW_RELEASE) {
}

KeyRepeatEvent::KeyRepeatEvent(const KeyContainer& keys, int key)
    : KeyEvent(Event::Type::KeyRepeat, keys, key, GLFW_REPEAT) {
}