//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "Event.h"
#include "../core/KeyCodes.h"

namespace engine {

    class KeyEvent : public Event {

    protected:
        KeyEvent(const int keycode)
        : keyCode(keycode) {

        }

    public:
        int getKeyCode() const { return keyCode; }

    public:
        EVENT_CLASS_CATEGORY(Keyboard | Input)

    protected:
        int keyCode;

    };

    class KeyPressedEvent : public KeyEvent {

    public:
        KeyPressedEvent(const int keycode, const uint16_t repeatCount)
        : KeyEvent(keycode), _repeatCount(repeatCount) {

        }

        uint16_t getRepeatCount() const { return _repeatCount; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode << " (" << _repeatCount << " repeats)";
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyPressed)

    private:
        uint16_t _repeatCount;

    };

    class KeyReleasedEvent : public KeyEvent {

    public:
        KeyReleasedEvent(const int keycode)
        : KeyEvent(keycode) {

        }

    public:
        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode;
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyReleased)

    };

    class KeyTypedEvent : public KeyEvent {

    public:
        KeyTypedEvent(const int keycode)
        : KeyEvent(keycode) {

        }

    public:
        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << keyCode;
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(KeyTyped)

    };

}