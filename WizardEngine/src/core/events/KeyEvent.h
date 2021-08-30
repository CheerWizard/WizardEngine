//
// Created by mecha on 29.08.2021.
//

#include "Event.h"
#include "../KeyCodes.h"

namespace engine {

    using namespace Key;

    class KeyEvent : public Event {

    protected:
        KeyEvent(const Code keycode)
        : _keyCode(keycode) {

        }

    public:
        KeyCode getKeyCode() const { return _keyCode; }

    public:
        EVENT_CLASS_CATEGORY(Keyboard | Input)

    private:
        Code _keyCode;

    };

    class KeyPressedEvent : public KeyEvent {

    public:
        KeyPressedEvent(const Code keycode, const uint16_t repeatCount)
        : KeyEvent(keycode), _repeatCount(repeatCount) {

        }

        uint16_t getRepeatCount() const { return _repeatCount; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyPressed)

    private:
        uint16_t _repeatCount;

    };

    class KeyReleasedEvent : public KeyEvent {

    public:
        KeyReleasedEvent(const Code keycode)
        : KeyEvent(keycode) {

        }

    public:
        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << _keyCode;
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyReleased)

    };

    class KeyTypedEvent : public KeyEvent {

    public:
        KeyTypedEvent(const Code keycode)
        : KeyEvent(keycode) {

        }

    public:
        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << _keyCode;
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyTyped)

    };

}