//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

namespace engine {

    Application* Application::_instance = nullptr;

    void Application::onCreate() {
        ENGINE_INFO("onCreate()");

        ENGINE_ASSERT(!_instance, "Application already created!");
        _instance = this;

        _window = Window::newInstance();
        _window->onCreate();

        _window->setEventCallback(this);
        _window->setWindowCallback(this);
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");

        _window->onDestroy();
    }

    void Application::run() {
        while (_isRunning) {
            onUpdate();
        }
    }

    void Application::onUpdate() {
        ENGINE_INFO("onUpdate()");

        Time deltaTime = Time();
        for (Layer* layer : _layerStack) {
            layer->onUpdate(deltaTime);
        }
        _window->onUpdate();
    }

    void Application::onEvent(Event &event) {
        auto eventName = event.toString();
        ENGINE_INFO("onEvent : {0}", eventName);

        for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
            if (event.isHandled) {
                break;
            }

            (*--it)->onEvent(event);
        }
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

}