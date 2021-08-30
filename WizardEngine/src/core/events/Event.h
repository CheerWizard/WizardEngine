//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <string>
#include <sstream>
#include "../Core.h"

namespace engine {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        App            = BIT(0),
        Win            = BIT(1),
        Input          = BIT(2),
        Keyboard       = BIT(3),
        Mouse          = BIT(4),
        MouseButton    = BIT(5)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    /**
     * Event interface, used to abstract every event type, name, etc.
     * As this class doesn't has a constructor, it is supposed to behave as interface.
     * */
    class Event {

    public:
        virtual ~Event() = default;

    public:

        virtual EventType getType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategory() const = 0;
        virtual std::string toString() const { return getName(); }

    public:

        bool hasCategory(EventCategory category) {
            return getCategory() & category;
        }

    public:
        bool isHandled = false;

    };

    class EventHandler {

    public:

        EventHandler(Event& event)
        : _event(event) {

        }

    public:

        template<typename T, typename F>
        bool handle(const F& func) {
            if (_event.getType() == T::GetStaticType()) {
                _event.isHandled |= func(static_cast<T&>(_event));
                return true;
            }
            return false;
        }

    private:
        Event& _event;

    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }

    class EventCallback {

    public:
        virtual void onEvent(Event& event) = 0;

    };

}