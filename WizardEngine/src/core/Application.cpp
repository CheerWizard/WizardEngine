//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

engine::Application::Application() {
    _window = Window::create();
    _window->setEventCallback(this);
}

engine::Application::~Application() {

}

void engine::Application::run() {
    while (_isRunning) {
        _window->onUpdate();
    }
}

void engine::Application::onEvent(engine::Event &event) {
    auto eventName = event.toString();
    ENGINE_INFO(eventName);
}
