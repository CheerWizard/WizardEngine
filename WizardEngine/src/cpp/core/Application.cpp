//
// Created by mecha on 28.08.2021.
//

#include <core/Application.h>

#include <graphics/camera/CameraShaderScript.h>
#include <graphics/material/MaterialShaderScript.h>
#include <graphics/light/LightShaderScript.h>

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
        createFrameSpecs();
        _layerStack.onPrepare();
        enableMSAA();
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
    }

    void Application::onWindowResized(const uint32_t &width , const uint32_t &height) {
        if (width == 0 || height == 0 || input->isMousePressed(MouseCode::ButtonLeft)) return;
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);

        _layerStack.onWindowResized(width, height);
        activeFrameController->resize(width, height);
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
        return WindowProps();
    }

    void Application::setWindowIcon(const std::string &filePath) {
        _window->setWindowIcon(filePath);
    }

    Ref<FileDialog> Application::createFileDialog() {
        return createRef<FileDialog>(_window->getNativeWindow());
    }

    void Application::createFrameSpecs() {
        activeFrameController->updateSpecs(getWindowWidth(), getWindowHeight());
        activeScene->setTextureId(activeFrameController->getFrameColors()[0]);
    }

    void Application::createGraphics() {
        GraphicsInitializer::createContext(_window->getNativeWindow());
        activeFrameController = createRef<FrameController>();
        // create batch renderer
        auto vBatchShader = shader::BaseShader({
           camera3dUboScript()
        });
        auto fBatchShader = shader::BaseShader({
           materialScript(),
           phongLightScript(),
           materialMapScript(),
           pointLightArrayScript()
        });
        auto batchShader = createRef<shader::BaseShaderProgram>(
            shader::ShaderProps {
                "batch",
                "v_batch.glsl",
                "f_batch.glsl",
                ENGINE_SHADERS_PATH
            },
            vBatchShader,
            fBatchShader
        );
        auto batchRenderer = createRef<Renderer>(batchShader);
        // create instance renderer
        auto vInstanceShader = shader::BaseShader({
            camera3dUboScript()
        });
        auto fInstanceShader = shader::BaseShader({
            materialScript(),
            phongLightScript(),
            materialMapScript(),
            pointLightArrayScript()
        });
        auto instanceShader = createRef<shader::BaseShaderProgram>(
            shader::ShaderProps {
                "instance",
                "v_instance.glsl",
                "f_instance.glsl",
                ENGINE_SHADERS_PATH
            },
            vInstanceShader,
            fInstanceShader
        );
        auto instanceRenderer = createRef<Renderer>(instanceShader);
        // create render system
        _renderSystem = createScope<RenderSystem>(activeFrameController, batchRenderer, instanceRenderer);
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
}