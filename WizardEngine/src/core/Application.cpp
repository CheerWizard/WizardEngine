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
        ENGINE_INFO("create()");

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

        _shaderCache = new ShaderCache();

        auto indexBuffer = _graphicsContext->newIndexBuffer();
        auto vertexArray = _graphicsContext->newVertexArray(new VertexBufferCache(), indexBuffer);

        _renderer = _graphicsContext->newRenderer(_shaderCache, _graphicsObjectCache, vertexArray);
        _renderer->onPrepare();
    }

    void Application::onDestroy() {
        ENGINE_INFO("destroy()");
        _renderer.reset();
        _window->onDestroy();
        _window.reset();
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

    void Application::addShader(const std::string &name, const Ref<Shader> &shader) {
        _shaderCache->add(name, shader);
    }

    void Application::addShader(const Ref<Shader> &shader) {
        _shaderCache->add(shader);
    }

    Ref<Shader> Application::loadShader(const std::string &filepath) {
        return _shaderCache->load(filepath);
    }

    Ref<Shader> Application::loadShader(const std::string &name, const std::string &filepath) {
        return _shaderCache->load(name, filepath);
    }

    Ref<Shader> Application::getShader(const std::string &name) {
        return _shaderCache->get(name);
    }

    bool Application::shaderExists(const std::string &name) const {
        return _shaderCache->exists(name);
    }

    void Application::loadVertices(const std::string &shaderName, const uint32_t &vertexStart, float *vertices) {
        _renderer->loadVertices(shaderName, vertexStart, vertices);
    }

    void Application::loadIndices(const std::string &shaderName, const uint32_t &indexStart, uint32_t *indices) {
        _renderer->loadIndices(shaderName, indexStart, indices);
    }

    void Application::loadObject(const std::string &shaderName, const Ref<GraphicsObject> &graphicsObject) {
        _graphicsObjectCache->add(shaderName, graphicsObject);
        _renderer->loadIndices(shaderName, graphicsObject->indicesSize, graphicsObject->indices);
        _renderer->loadVertices(shaderName, graphicsObject->verticesSize, graphicsObject->vertices);
    }

}