//
// Created by mecha on 29.08.2021.
//

#pragma once

#include <event/KeyCodes.h>
#include <event/MouseCodes.h>
#include <functional>

namespace engine::event {

    // just a wrapper of functional object
    template<typename ...T>
    struct Action {
        std::function<void(T...)> function;

        Action() {
            function = [](T...){};
        }

        Action(const Action<T...>& action) = default;

        Action(const std::function<void(T...)>&& function) : function(function) {}
    };

    // registry of window and input events.
    // maps window and input event with appropriate action function.
    struct EventRegistry {
        Action<> onWindowClosed;
        Action<const uint32_t&, const uint32_t&> onWindowResized;

        typedef std::unordered_map<KeyCode, Action<KeyCode>> KeyCodeMap;
        KeyCodeMap onKeyPressedMap;
        KeyCodeMap onKeyHoldMap;
        KeyCodeMap onKeyReleasedMap;
        KeyCodeMap onKeyTypedMap;

        typedef std::unordered_map<MouseCode, Action<MouseCode>> MouseCodeMap;
        MouseCodeMap onMousePressedMap;
        MouseCodeMap onMouseReleasedMap;

        Action<double, double> onMouseScrolled;
        Action<double, double> onCursorMoved;
    };

}