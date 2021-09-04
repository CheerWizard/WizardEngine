//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

namespace engine {

    Application* Application::_instance = nullptr;

    void Application::run() {
        onCreate();
        while (_isRunning) {
            onUpdate();
        }
        onDestroy();
    }

    void Application::onCreate() {
        ENGINE_INFO("onCreate()");

        ENGINE_ASSERT(!_instance, "Application already created!");
        _instance = this;

        _window = Window::newInstance();
        _window->onCreate();

        _window->setWindowCallback(this);
        _window->setMouseCallback(this);
        _window->setKeyboardCallback(this);
        _window->setCursorCallback(this);
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");

        _window->onDestroy();
    }

    void Application::onUpdate() {
        ENGINE_INFO("onUpdate()");

        Time deltaTime = Time();
        _layerStack.onUpdate(deltaTime);
        _window->onUpdate();
    }

    void Application::pushLayer(Layer *layer) {
        ENGINE_INFO("Pushing layer : {0}", layer->getName());
        _layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer *overlay) {
        ENGINE_INFO("Pushing overlay : {0}", overlay->getName());
        _layerStack.pushOverlay(overlay);
    }

    void Application::onWindowClosed() {
        ENGINE_INFO("Application : onWindowClosed()");
        _isRunning = false;
    }

    void Application::onWindowResized(unsigned int width, unsigned int height) {
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);
    }

    void Application::onKeyPressed(KeyCode keyCode) {

    }

    void Application::onKeyHold(KeyCode keyCode) {

    }

    void Application::onKeyReleased(KeyCode keyCode) {

    }

    void Application::onMousePressed(MouseCode mouseCode) {

    }

    void Application::onMouseRelease(MouseCode mouseCode) {

    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {

    }

    void Application::onCursorMoved(double xPos, double yPos) {

    }

}