//
// Created by mecha on 28.08.2021.
//

#include "Application.h"

#include "../platform/Platform.h"

namespace engine {

    void Application::run() {
        onCreate();
        onPrepare();
        while (_isRunning) {
            onUpdate();
        }
        onDestroy();
    }

    void Application::onCreate() {
        ENGINE_INFO("create()");

        _window = INIT_WINDOW(createWindowProps());

        fpsController.setMaxFps(getRefreshRate());

        _graphicsContext = INIT_GRAPHICS_CONTEXT(_window->getNativeWindow());
        _graphicsContext->onCreate();
        _graphicsContext->printInfo();
        createGraphics();

        _window->setWindowCallback(this);
        _window->setMouseCallback(this);
        _window->setKeyboardCallback(this);
        _window->setCursorCallback(this);

        input = INIT_INPUT(_window->getNativeWindow());

        createActiveScene();
    }

    void Application::onPrepare() {
        _window->onPrepare();
        createFrameSpecs();
        _layerStack.onPrepare();
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");
    }

    void Application::onUpdate() {
        auto dt = fpsController.getDeltaTime();
        fpsController.begin();

        _renderSystem->onUpdate();

        _layerStack.onUpdate(dt);
        _window->onUpdate();
        _graphicsContext->swapBuffers();

        fpsController.end();
    }

    void Application::pushFront(Layer *layer) {
        ENGINE_INFO("Pushing layer : {0}", layer->getTag());
        _layerStack.pushFront(layer);
    }

    void Application::pushBack(Layer *layer) {
        ENGINE_INFO("Pushing overlay : {0}", layer->getTag());
        _layerStack.pushBack(layer);
    }

    void Application::onWindowClosed() {
        ENGINE_INFO("Application : onWindowClosed()");
        _layerStack.onWindowClosed();
        _isRunning = false;
    }

    void Application::onWindowResized(const uint32_t &width , const uint32_t &height) {
        if (width == 0 || height == 0 || input->isMousePressed(MouseCode::ButtonLeft)) return;
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);

        _layerStack.onWindowResized(width, height);
        activeFrameController->resize(width, height);
    }

    void Application::onKeyPressed(KeyCode keyCode) {
        if (_closeKeyPressed == keyCode) {
            onWindowClosed();
        }
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

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    void Application::createActiveScene() {
        activeScene = createRef<Scene>();
        _renderSystem->setActiveScene(activeScene);
    }

    void Application::restart() {
        onDestroy();
        onCreate();
    }

    const uint32_t &Application::getWindowWidth() {
        return _window->getWidth();
    }

    const uint32_t &Application::getWindowHeight() {
        return _window->getHeight();
    }

    uint32_t Application::getRefreshRate() {
        return _window->getRefreshRate();
    }

    void *Application::getNativeWindow() {
        return _window->getNativeWindow();
    }

    WindowProps Application::createWindowProps() {
        return WindowProps();
    }

    void Application::setWindowIcon(const std::string &filePath) {
        _window->setWindowIcon(filePath);
    }

    Ref<FileDialog> Application::createFileDialog() {
        return INIT_FILE_DIALOG(_window->getNativeWindow());
    }

    void Application::createSources() {
        _textureSource = createRef<TextureSource>(_graphicsFactory);
        _shaderSource = createRef<ShaderSource>(_graphicsFactory);
    }

    void Application::createFrameSpecs() {
        activeFrameController->updateSpecs(getWindowWidth(), getWindowHeight());
        activeScene->setTextureId(activeFrameController->getFrameColors()[0]);
    }

    void Application::createGraphics() {
        _graphicsFactory = _graphicsContext->newGraphicsFactory();
        activeFrameController = createRef<FrameController>(_graphicsFactory->newFrameBuffer());
        createSources();
        _renderSystem = createRef<RenderSystem>(_graphicsFactory, activeFrameController, _shaderSource, _textureSource);
    }
}