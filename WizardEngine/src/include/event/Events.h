//
// Created by mecha on 29.08.2021.
//

#pragma once

#include <event/KeyCodes.h>
#include <event/MouseCodes.h>
#include <event/GamepadCodes.h>
#include <core/map.h>
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
    class EventRegistry final {

        typedef core::unordered_map<KeyCode, Action<KeyCode>> KeyCodeMap;
        typedef core::unordered_map<KeyCode, bool> KeyHoldMap;
        typedef core::unordered_map<MouseCode, Action<MouseCode>> MouseCodeMap;
        typedef core::unordered_map<MouseCode, bool> MouseHoldMap;
        typedef core::unordered_map<GamepadButtonCode, Action<GamepadButtonCode>> GamepadButtonMap;

    public:
        static Action<> onWindowClosed;
        static Action<const uint32_t&, const uint32_t&> onWindowResized;

        static KeyCodeMap onKeyPressedMap;
        static KeyHoldMap onKeyHoldMap;
        static KeyCodeMap onKeyReleasedMap;
        static KeyCodeMap onKeyTypedMap;

        static MouseCodeMap onMousePressedMap;
        static MouseCodeMap onMouseReleasedMap;

        static MouseHoldMap mouseHoldMap;

        static Action<double, double> onMouseScrolled;
        static Action<double, double> onCursorMoved;

        static GamepadButtonMap onGamepadButtonPressedMap;
        static GamepadButtonMap onGamepadButtonReleasedMap;

        static Action<GamepadRoll> onGamepadRollLeft;
        static Action<GamepadRoll> onGamepadRollRight;
        static GamepadRoll inactiveGamepadRollLeft;
        static GamepadRoll inactiveGamepadRollRight;

        static bool keyHold(KeyCode keyCode);
        static bool mouseHold(MouseCode mouseCode);
    };
}