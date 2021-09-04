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

        window = Window::newInstance();
        window->onCreate();

        window->setWindowCallback(this);
        window->setMouseCallback(this);
        window->setKeyboardCallback(this);
        window->setCursorCallback(this);

        input = Input::newInstance();
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");
        window->onDestroy();
    }

    void Application::onUpdate() {
        ENGINE_INFO("onUpdate()");
        Time deltaTime = Time();
        _layerStack.onUpdate(deltaTime);
        window->onUpdate();
        input->getMousePosition().log();
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
        _layerStack.onWindowClosed();
        _isRunning = false;
    }

    void Application::onWindowResized(unsigned int width, unsigned int height) {
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);
        _layerStack.onWindowResized(width, height);
    }

    void Application::onKeyPressed(KeyCode keyCode) {
        _layerStack.onKeyPressed(keyCode);
    }

    void Application::onKeyHold(KeyCode keyCode) {
        _layerStack.onKeyHold(keyCode);
    }

    void Application::onKeyReleased(KeyCode keyCode) {
        _layerStack.onKeyReleased(keyCode);
    }

    void Application::onMousePressed(MouseCode mouseCode) {
        _layerStack.onMousePressed(mouseCode);
    }

    void Application::onMouseRelease(MouseCode mouseCode) {
        _layerStack.onMouseRelease(mouseCode);
    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {
        _layerStack.onMouseScrolled(xOffset, yOffset);
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        _layerStack.onCursorMoved(xPos, yPos);
    }

    void Application::onKeyTyped(KeyCode keyCode) {
        _layerStack.onKeyTyped(keyCode);
    }

}