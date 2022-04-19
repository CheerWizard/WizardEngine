//
// Created by mecha on 28.08.2021.
//

#include <core/Application.h>

namespace engine::core {

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

        input = createScope<event::Input>(_window->getNativeWindow());

        createScripting();

        setActiveScene(createRef<Scene>());
    }

    void Application::onPrepare() {
        RenderCommands::logApiInfo();
        _window->onPrepare();
        _window->setInCenter();
        setSampleSize(1);
        _layerStack.onPrepare();
        loadGamepadMappings("../WizardEngine/assets/db/game_controller_db.txt");
    }

    void Application::onDestroy() {
        ENGINE_INFO("onDestroy()");
        _scriptSystem->onDestroy();
    }

    void Application::onUpdate() {
        auto dt = fpsController.getDeltaTime();
        fpsController.begin();
        updateEventRegistry();
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
        eventRegistry.onWindowClosed.function();
        shutdown();
    }

    void Application::onWindowResized(const u32 &width , const u32 &height) {
        if (width == 0 || height == 0 || input->isMousePressed(event::MouseCode::ButtonLeft)) return;
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);

        _layerStack.onWindowResized(width, height);
        activeSceneFrame->resize(width, height);
        screenFrame->resize(width, height);
        eventRegistry.onWindowResized.function(width, height);
    }

    void Application::onKeyPressed(event::KeyCode keyCode) {
        _layerStack.onKeyPressed(keyCode);
        eventRegistry.onKeyPressedMap[keyCode].function(keyCode);
    }

    void Application::onKeyHold(event::KeyCode keyCode) {
        _layerStack.onKeyHold(keyCode);
        eventRegistry.onKeyHoldMap[keyCode].function(keyCode);
    }

    void Application::onKeyReleased(event::KeyCode keyCode) {
        _layerStack.onKeyReleased(keyCode);
        eventRegistry.onKeyReleasedMap[keyCode].function(keyCode);
    }

    void Application::onMousePressed(event::MouseCode mouseCode) {
        _layerStack.onMousePressed(mouseCode);
        eventRegistry.onMousePressedMap[mouseCode].function(mouseCode);
    }

    void Application::onMouseRelease(event::MouseCode mouseCode) {
        _layerStack.onMouseRelease(mouseCode);
        eventRegistry.onMouseReleasedMap[mouseCode].function(mouseCode);
    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {
        _layerStack.onMouseScrolled(xOffset, yOffset);
        eventRegistry.onMouseScrolled.function(xOffset, yOffset);
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        _layerStack.onCursorMoved(xPos, yPos);
        eventRegistry.onCursorMoved.function(xPos, yPos);
    }

    void Application::onKeyTyped(event::KeyCode keyCode) {
        _layerStack.onKeyTyped(keyCode);
    }

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    void Application::setActiveScene(const Ref<Scene>& scene) {
        scenes.emplace_back(scene);
        activeScene = scene;
        _renderSystem->setActiveScene(scene);
        _scriptSystem->setActiveScene(scene);
    }

    void Application::setActiveScene(const u32 &sceneIndex) {
        activeScene = scenes[sceneIndex];
        _scriptSystem->setActiveScene(activeScene);
        _renderSystem->setActiveScene(activeScene);
        _renderSystem->onUpdate();
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

    Ref<tools::FileDialog> Application::createFileDialog() {
        return createRef<tools::FileDialog>(_window->getNativeWindow());
    }

    void Application::setSampleSize(const uint32_t& samples) {
        _window->setSampleSize(samples);
        // update active scene fbo
        FrameBufferFormat activeSceneFrameFormat;
        activeSceneFrameFormat.colorAttachments = {
                { graphics::ColorFormat::RGBA8 }
        };
        activeSceneFrameFormat.renderBufferAttachment = { DepthStencilFormat::DEPTH24STENCIL8 };
        activeSceneFrameFormat.width = _window->getWidth();
        activeSceneFrameFormat.height = _window->getHeight();
        activeSceneFrameFormat.samples = samples;
        activeSceneFrame->updateFormat(activeSceneFrameFormat);
        // update screen fbo
        FrameBufferFormat screenFrameFormat;
        screenFrameFormat.colorAttachments = {
                { graphics::ColorFormat::RGBA8 }
        };
        screenFrameFormat.width = _window->getWidth();
        screenFrameFormat.height = _window->getHeight();
        screenFrameFormat.samples = 1;
        screenFrame->updateFormat(screenFrameFormat);
        // resolve size issue
        onWindowResized(_window->getWidth(), _window->getHeight());
    }

    void Application::createGraphics() {
        graphics::initContext(_window->getNativeWindow());
        activeSceneFrame = createRef<FrameBuffer>();
        screenFrame = createRef<FrameBuffer>();
        _renderSystem = createScope<RenderSystem>(activeSceneFrame, screenFrame);
    }

    void Application::updateRuntime(Time dt) {
        if (!activeScene->isEmpty()) {
            _scriptSystem->onUpdate(dt);
            _renderSystem->onUpdate();
        } else {
            ENGINE_WARN("Active scene is empty!");
        }
    }

    void Application::shutdown() {
        _isRunning = false;
    }

    void Application::createScripting() {
        _scriptSystem = createScope<scripting::ScriptSystem>();
    }

    void Application::onGamepadConnected(s32 joystickId) {
        ENGINE_INFO("onGamepadConnected(id: {0})", joystickId);
        input->setJoystickId(joystickId);
        isJoystickConnected = true;
    }

    void Application::onGamepadDisconnected(s32 joystickId) {
        ENGINE_INFO("onGamepadDisconnected(id: {0})", joystickId);
        input->setJoystickId(joystickId);
        isJoystickConnected = false;
    }

    Application* Application::instance = nullptr;

    Application::Application() {
        instance = this;
    }

    Application::~Application() {
        instance = nullptr;
    }

    void Application::loadGamepadMappings(const char *mappingsFilePath) {
        _window->loadGamepadMappings(mappingsFilePath);
    }

    void Application::updateEventRegistry() {
        if (isJoystickConnected) {
            ENGINE_INFO("Joystick is connected!");
            auto gamepadState = input->getGamepadState();
            auto& buttons = gamepadState.buttons;
            auto& axis = gamepadState.axes;

            for (auto& gamepadButtonPressed : eventRegistry.onGamepadButtonPressedMap) {
                if (buttons[gamepadButtonPressed.first] == event::PRESS) {
                    gamepadButtonPressed.second.function(gamepadButtonPressed.first);
                }
            }

            for (auto& gamepadButtonReleased : eventRegistry.onGamepadButtonReleasedMap) {
                if (buttons[gamepadButtonReleased.first] == event::RELEASE) {
                    gamepadButtonReleased.second.function(gamepadButtonReleased.first);
                }
            }

            for (auto& gamepadAxis : eventRegistry.gamepadAxisMap) {
                gamepadAxis.second.function(axis[gamepadAxis.first]);
            }
        } else {
            ENGINE_INFO("Joystick is disconnected!");
            isJoystickConnected = input->isJoystickConnected();
        }
    }
}