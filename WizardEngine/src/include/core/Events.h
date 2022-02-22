//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include <functional>

namespace engine {

    template<typename T>
    struct WindowCallback final {
        T* callback = nullptr;
        std::function<void()> onWindowClosed;
        std::function<void(const uint32_t&, const uint32_t&)> onWindowResized;

        WindowCallback(T* callback) : callback(callback) {
            bind();
        }

        void bind() {
            onWindowClosed = [this]() { callback->onWindowClosed(); };
            onWindowResized = [this](const uint32_t &w, const uint32_t &h) { callback->onWindowResized(w, h); };
        }
    };

    template<typename T>
    struct KeyboardCallback final {
        T* callback = nullptr;
        std::function<void(KeyCode)> onKeyPressed;
        std::function<void(KeyCode)> onKeyHold;
        std::function<void(KeyCode)> onKeyReleased;
        std::function<void(KeyCode)> onKeyTyped;

        KeyboardCallback(T* callback) : callback(callback) {
            bind();
        }

        void bind() {
            onKeyPressed = [this](KeyCode keyCode) { callback->onKeyPressed(keyCode); };
            onKeyHold = [this](KeyCode keyCode) { callback->onKeyHold(keyCode); };
            onKeyReleased = [this](KeyCode keyCode) { callback->onKeyReleased(keyCode); };
            onKeyTyped = [this](KeyCode keyCode) { callback->onKeyTyped(keyCode); };
        }
    };

    template<typename T>
    struct MouseCallback final {
        T* callback = nullptr;
        std::function<void(MouseCode)> onMousePressed;
        std::function<void(MouseCode)> onMouseRelease;
        std::function<void(double, double)> onMouseScrolled;

        MouseCallback(T* callback) : callback(callback) {
            bind();
        }

        void bind() {
            onMousePressed = [this](MouseCode mouseCode) { callback->onMousePressed(mouseCode); };
            onMouseRelease = [this](MouseCode mouseCode) { callback->onMouseRelease(mouseCode); };
            onMouseScrolled = [this](double x, double y) { callback->onMouseScrolled(x, y); };
        }
    };

    template<typename T>
    struct CursorCallback final {
        T* callback = nullptr;
        std::function<void(double, double)> onCursorMoved;

        CursorCallback(T* callback) : callback(callback) {
            bind();
        }

        void bind() {
            onCursorMoved = [this](double x, double y) { callback->onCursorMoved(x, y); };
        }
    };

}