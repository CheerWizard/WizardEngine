//
// Created by mecha on 29.08.2021.
//

#include "Event.h"

namespace engine {

    class WindowResizeEvent : public Event {

    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
        : _width(width), _height(height) {

        }

    public:
        unsigned int getWidth() const { return _width; }
        unsigned int getHeight() const { return _height; }

    public:
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << _width << ", " << _height;
            return ss.str();
        }

    public:
        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(Win)

    private:
        unsigned int _width, _height;

    };

    class WindowCloseEvent : public Event {

    public:
        WindowCloseEvent() = default;

    public:
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(Win)

    };

}