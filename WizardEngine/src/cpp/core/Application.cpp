//
// Created by mecha on 28.08.2021.
//

#include <core/Application.h>

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
        ENGINE_INFO("onCreate()");

        _window = createScope<Window>(createWindowProps());

        fpsController.setMaxFps(getRefreshRate());

        createGraphics();

        WINDOW_CALLBACK(_window, this);
        KEYBOARD_CALLBACK(_window, this);
        MOUSE_CALLBACK(_window, this);
        CURSOR_CALLBACK(_window, this);

        input = createScope<Input>(_window->getNativeWindow());

        createScripting();

        createActiveScene();
    }

    void Application::onPrepare() {
        RenderCommands::logApiInfo();
        _window->onPrepare();
        _window->setInCenter();
        setSampleSize(1);
        _layerStack.onPrepare();
        setMSAA(true);
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");
        _scriptSystem->onDestroy();
    }

    void Application::onUpdate() {
        auto dt = fpsController.getDeltaTime();
        fpsController.begin();
        // draw runtime systems
        updateRuntime(dt);
        // draw editor/tools
        _layerStack.onUpdate(dt);
        // poll events + swap chain
        _window->onUpdate();

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
        eventController.onWindowClosed.function();
    }

    void Application::onWindowResized(const uint32_t &width , const uint32_t &height) {
        if (width == 0 || height == 0 || input->isMousePressed(MouseCode::ButtonLeft)) return;
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);

        _layerStack.onWindowResized(width, height);
//        activeSceneFrame->resize(width, height);
//        screenFrame->resize(width, height);
        eventController.onWindowResized.function(width, height);
    }

    void Application::onKeyPressed(KeyCode keyCode) {
        _layerStack.onKeyPressed(keyCode);
        eventController.onKeyPressedMap[keyCode].function(keyCode);
    }

    void Application::onKeyHold(KeyCode keyCode) {
        _layerStack.onKeyHold(keyCode);
        eventController.onKeyHoldMap[keyCode].function(keyCode);
    }

    void Application::onKeyReleased(KeyCode keyCode) {
        _layerStack.onKeyReleased(keyCode);
        eventController.onKeyReleasedMap[keyCode].function(keyCode);
    }

    void Application::onMousePressed(MouseCode mouseCode) {
        _layerStack.onMousePressed(mouseCode);
        eventController.onMousePressedMap[mouseCode].function(mouseCode);
    }

    void Application::onMouseRelease(MouseCode mouseCode) {
        _layerStack.onMouseRelease(mouseCode);
        eventController.onMouseReleasedMap[mouseCode].function(mouseCode);
    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {
        _layerStack.onMouseScrolled(xOffset, yOffset);
        eventController.onMouseScrolled.function(xOffset, yOffset);
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        _layerStack.onCursorMoved(xPos, yPos);
        eventController.onCursorMoved.function(xPos, yPos);
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
        _scriptSystem->setActiveScene(activeScene);
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
        return {};
    }

    void Application::setWindowIcon(const std::string &filePath) {
        _window->setWindowIcon(filePath);
    }

    Ref<FileDialog> Application::createFileDialog() {
        return createRef<FileDialog>(_window->getNativeWindow());
    }

    void Application::setSampleSize(const uint32_t& samples) {
        _window->setSampleSize(samples);
        // update active scene fbo
        engine::FrameBufferFormat activeSceneFrameFormat;
        activeSceneFrameFormat.colorAttachments = {
                { engine::ColorFormat::RGBA8 }
        };
        activeSceneFrameFormat.renderBufferAttachment = {DepthStencilFormat::DEPTH24STENCIL8 };
        activeSceneFrameFormat.width = _window->getWidth();
        activeSceneFrameFormat.height = _window->getHeight();
        activeSceneFrameFormat.samples = samples;
        activeSceneFrame->updateFormat(activeSceneFrameFormat);
        // update screen fbo
        FrameBufferFormat screenFrameFormat;
        screenFrameFormat.colorAttachments = {
                { engine::ColorFormat::RGBA8 }
        };
        screenFrameFormat.width = _window->getWidth();
        screenFrameFormat.height = _window->getHeight();
        screenFrameFormat.samples = 1;
        screenFrame->updateFormat(screenFrameFormat);

        activeScene->setTextureId(screenFrame->getColorAttachment(0).id);
    }

    void Application::createGraphics() {
        GraphicsInitializer::createContext(_window->getNativeWindow());
        activeSceneFrame = createRef<FrameBuffer>();
        screenFrame = createRef<FrameBuffer>();
        _renderSystem = createScope<RenderSystem>(activeSceneFrame, screenFrame);
    }

    void Application::updateRuntime(Time dt) {
        if (activeScene != nullptr && !activeScene->isEmpty()) {
            _scriptSystem->onUpdate(dt);
            _renderSystem->onUpdate();
        } else {
            ENGINE_WARN("Active scene is null or empty!");
        }
    }

    void Application::shutdown() {
        _isRunning = false;
    }

    void Application::createScripting() {
        _scriptSystem = createScope<ScriptSystem>();
    }

    void Application::setSkybox(const Entity& skybox) {
        _renderSystem->setSkybox(skybox);
    }
}