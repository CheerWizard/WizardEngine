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
        onPrepare();
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
        _graphicsContext->setViewPort(_window->getWidth(), _window->getHeight());

        _window->setWindowCallback(this);
        _window->setMouseCallback(this);
        _window->setKeyboardCallback(this);
        _window->setCursorCallback(this);

        input = INIT_INPUT;

        _imGuiLayer = new ImGuiLayer();
        pushOverlay(_imGuiLayer);

        createRenderSystem();
        createActiveScene();
        createCamera();

        objFile = new ObjFile();
    }

    void Application::onPrepare() {
        _window->onPrepare();
        _renderSystem->onPrepare();
    }

    void Application::onDestroy() {
        ENGINE_INFO("destroy()");
        delete objFile;
        delete cameraController;
        delete activeScene;
        input.reset();
        _renderSystem.reset();
        _window->onDestroy();
        _window.reset();
    }

    void Application::onUpdate() {
        Time deltaTime = Time();
        _renderSystem->onUpdate();
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
        _graphicsContext->setViewPort(width, height);
        _layerStack.onWindowResized(width, height);
    }

    void Application::onKeyPressed(KeyCode keyCode) {
        if (closeKeyPressed == keyCode) {
            onWindowClosed();
        }
        _layerStack.onKeyPressed(keyCode);
        cameraController->onKeyPressed(keyCode);
    }

    void Application::onKeyHold(KeyCode keyCode) {
        _layerStack.onKeyHold(keyCode);
        cameraController->onKeyHold(keyCode);
    }

    void Application::onKeyReleased(KeyCode keyCode) {
        _layerStack.onKeyReleased(keyCode);
        cameraController->onKeyReleased(keyCode);
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
        cameraController->onKeyTyped(keyCode);
    }

    void Application::pushLayout(Layout *imGuiLayout) {
        _imGuiLayer->pushLayout(imGuiLayout);
    }

    void Application::pushOverLayout(Layout *imGuiLayout) {
        _imGuiLayer->pushOverLayout(imGuiLayout);
    }

    void Application::addShader(const std::string &name, const Ref<Shader> &shader) {
        _renderSystem->addShader(name, shader);
    }

    void Application::addShader(const Ref<Shader> &shader) {
        _renderSystem->addShader(shader);
    }

    ShaderError Application::loadShader(const ShaderProps &shaderProps, VertexFormat* vertexFormat) {
        return _renderSystem->loadShader(shaderProps, vertexFormat);
    }

    ShaderError Application::loadShader(const ShaderProps &shaderProps) {
        return _renderSystem->loadShader(shaderProps);
    }

    Ref<Shader> Application::getShader(const std::string &name) {
        return _renderSystem->getShader(name);
    }

    bool Application::shaderExists(const std::string &name) const {
        return _renderSystem->shaderExists(name);
    }

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    void Application::enableDepthRendering() {
        _graphicsContext->enableDepth();
    }

    void Application::loadTexture(const std::string &filePath) {
        _renderSystem->loadTexture(filePath);
    }

    void Application::loadTextureData(const void *data) {
        _renderSystem->loadTextureData(data);
    }

    void Application::createCamera3D(const char *name) {
        createCamera3D(name, {0.5, 0.5, 1});
    }

    void Application::createCamera3D(const char *name, const glm::vec3 &position) {
        auto camera3D = new engine::Camera3d {
            name,
            engine::ViewMatrix3d {
                "view3d",
                position
                },
                engine::PerspectiveMatrix {
                "projection3d",
                getAspectRatio()
            }
        };
        createCamera3D(camera3D);
    }

    void Application::createCamera3D(Camera3d* camera3D) {
        cameraController = new Camera3dController(camera3D);
        _renderSystem->setCameraController(cameraController);
    }

    void Application::createActiveScene() {
        activeScene = new Scene();
        _renderSystem->setActiveScene(activeScene);
    }

    void Application::createCamera2D(const char *name) {
        createCamera2D(name, {0, 0, 1});
    }

    void Application::createCamera2D(const char *name, const glm::vec3 &position) {
        auto camera2D = new engine::Camera2d {
            name,
            engine::ViewMatrix2d {
                "view2d",
                position
                },
                engine::OrthographicMatrix {
                "projection2d",
                getAspectRatio()
            }
        };
        createCamera2D(camera2D);
    }

    void Application::createCamera2D(Camera2d *camera2D) {
        cameraController = new Camera2dController(camera2D);
        _renderSystem->setCameraController(cameraController);
    }

    void Application::restart() {
        onDestroy();
        onCreate();
    }

    void Application::createCamera(const char *name) {
        if (_engineType == ENGINE_2D) {
            createCamera2D(name);
        } else {
            createCamera3D(name);
        }
    }

    void Application::createRenderSystem() {
        if (_engineType == ENGINE_2D) {
            _renderSystem = _graphicsContext->newRenderSystem2d();
        } else {
            _renderSystem = _graphicsContext->newRenderSystem3d();
        }
    }

    ObjData Application::loadObj(const std::string &objName) {
        return objFile->readObj(objName);
    }

}