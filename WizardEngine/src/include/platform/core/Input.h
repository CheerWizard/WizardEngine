//
// Created by mecha on 04.09.2021.
//

#pragma once

#include <core/primitives.h>
#include <event/KeyCodes.h>
#include <event/MouseCodes.h>

namespace engine::event {

    using namespace core;

    struct MousePosition {
        f32 x;
        f32 y;
    };

    struct JoystickAxisStates {
        const f32* axes;
        s32 count;
    };

    struct JoystickButtonStates {
        const u8* buttons;
        s32 count;
    };

    struct JoystickHatStates {
        const u8* hats;
        s32 count;
    };

    struct GamepadState {
        u8* buttons;
        f32* axes;
    };

    class Input {

    public:
        Input(void* nativeWindow) : nativeWindow(nativeWindow) {}

        ~Input() {
            destroy();
        }

    public:
        bool isKeyPressed(KeyCode keyCode);
        bool isMousePressed(MouseCode mouseCode);
        MousePosition getMousePosition();
        [[nodiscard]] bool isJoystickConnected() const;
        [[nodiscard]] bool isJoystickDisconnected() const;
        [[nodiscard]] JoystickAxisStates getJoystickAxisStates() const;
        [[nodiscard]] JoystickButtonStates getJoystickButtonStates() const;
        [[nodiscard]] JoystickHatStates getJoystickHatStates() const;
        [[nodiscard]] const char* getJoystickName() const;
        [[nodiscard]] bool isGamepadPresent() const;
        [[nodiscard]] const char* getGamepadName() const;
        [[nodiscard]] GamepadState getGamepadState() const;

    public:
        inline f32 getMousePosX() {
            return getMousePosition().x;
        }

        inline f32 getMousePosY() {
            return getMousePosition().y;
        }

        inline void setJoystickId(const s32& joystickId) {
            this->joystickId = joystickId;
        }

    private:
        void destroy();

    private:
        void* nativeWindow;
        s32 joystickId = 0;
    };
}
