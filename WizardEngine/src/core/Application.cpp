//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

namespace engine {

    Application* Application::_instance = nullptr;

    Application::Application() {
        ENGINE_ASSERT(!_instance, "Application already created!");
        _instance = this;

        _window = Window::newInstance();
        _window->setEventCallback(this);
    }

    Application::~Application() = default;

    void Application::run() {
        Time deltaTime = Time();
        while (_isRunning) {
            for (Layer* layer : _layerStack) {
                layer->onUpdate(deltaTime);
            }
            _window->onUpdate();
        }
    }

    void Application::onEvent(Event &event) {
        auto eventName = event.toString();
        ENGINE_INFO("onEvent : {0}", eventName);

        for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
            (*--it)->onEvent(event);

            if (event.isHandled) {
                break;
            }
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

}