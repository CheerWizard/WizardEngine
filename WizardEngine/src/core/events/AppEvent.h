//
// Created by mecha on 29.08.2021.
//

#include "Event.h"

namespace engine {

    class AppTickEvent : public Event {

    public:
        AppTickEvent() = default;

    public:
        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(App)

    };

    class AppUpdateEvent : public Event {

    public:
        AppUpdateEvent() = default;

    public:
        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(App)

    };

    class AppRenderEvent : public Event {

    public:
        AppRenderEvent() = default;

    public:
        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(App)

    };

}
