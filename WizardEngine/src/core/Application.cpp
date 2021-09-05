//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

#include "../platform/windows/WindowsWindow.h"
#include "../platform/windows/WindowsInput.h"

#include "../platform/opengl/GLContext.h"

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

        ENGINE_ASSERT(!_instance, "Application already created!")
        _instance = this;

        _window = INIT_WINDOW(WindowProps());
        _window->onCreate();

        _graphicsContext = INIT_GRAPHICS_CONTEXT;
        _graphicsContext->onCreate();
        _graphicsContext->printInfo();

        _window->setWindowCallback(this);
        _window->setMouseCallback(this);
        _window->setKeyboardCallback(this);
        _window->setCursorCallback(this);
        _window->onPrepare();

        input = INIT_INPUT;

        _imGuiLayer = new ImGuiLayer();
        pushOverlay(_imGuiLayer);

        _renderer = _graphicsContext->newRenderer();
        _renderer->onCreate();
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");
        _renderer->onDestroy();
        _window->onDestroy();
    }

    void Application::onUpdate() {
        Time deltaTime = Time();
        _renderer->onUpdate();
        _layerStack.onUpdate(deltaTime);
        _window->onUpdate();
        _graphicsContext->swapBuffers();
        _graphicsContext->clearDisplay();
    }

    void Application::pushLayer(Layer *layer) {
        ENGINE_INFO("Pushing layer : {0}", layer->getTag());
        _layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer *overlay) {
        ENGINE_INFO("Pushing overlay : {0}", overlay->getTag());
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

    void Application::pushLayout(Layout *imGuiLayout) {
        _imGuiLayer->pushLayout(imGuiLayout);
    }

    void Application::pushOverLayout(Layout *imGuiLayout) {
        _imGuiLayer->pushOverLayout(imGuiLayout);
    }

}