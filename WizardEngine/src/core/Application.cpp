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

        _window->setWindowCallback(this);
        _window->setMouseCallback(this);
        _window->setKeyboardCallback(this);
        _window->setCursorCallback(this);

        input = INIT_INPUT;

        _imGuiLayer = new ImGuiLayer();
        pushOverlay(_imGuiLayer);

        auto indexBuffer = _graphicsContext->newIndexBuffer();
        auto vertexArray = _graphicsContext->newVertexArray(new VertexBufferCache(), indexBuffer);

        _renderer = _graphicsContext->newRenderer(
                new ShaderCache(),
                new GraphicsObjectCache(),
                vertexArray);

        createCamera3D("camera");
    }

    void Application::onPrepare() {
        _window->onPrepare();
        _renderer->onPrepare();
    }

    void Application::onDestroy() {
        ENGINE_INFO("destroy()");
        delete cameraController;
        _renderer.reset();
        input.reset();
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
        _renderer->addShader(name, shader);
    }

    void Application::addShader(const Ref<Shader> &shader) {
        _renderer->addShader(shader);
    }

    Ref<Shader> Application::loadShader(const ShaderProps &shaderProps, VertexFormat* vertexFormat) {
        return _renderer->loadShader(shaderProps, vertexFormat);
    }

    Ref<Shader> Application::getShader(const std::string &name) {
        return _renderer->getShader(name);
    }

    bool Application::shaderExists(const std::string &name) const {
        return _renderer->shaderExists(name);
    }

    void Application::loadObject(const Ref<GraphicsObject> &graphicsObject) {
        _renderer->loadObject(graphicsObject);
    }

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    uint32_t Application::addObject(const Ref<GraphicsObject> &graphicsObject) {
        return _renderer->addObject(graphicsObject);
    }

    void Application::updateObject(const Ref<GraphicsObject> &graphicsObject) {
        _renderer->updateObject(graphicsObject);
    }

    const Ref<GraphicsObject>& Application::getGraphicsObject(const std::string &shaderName,
                                                              const uint32_t &objectIndex) {
        return _renderer->getGraphicsObject(shaderName, objectIndex);
    }

    void Application::enableDepthRendering() {
        _graphicsContext->enableDepth();
    }

    void Application::loadTexture(const std::string &filePath) {
        _renderer->loadTexture(filePath);
    }

    void Application::loadTextureData(const void *data) {
        _renderer->loadTextureData(data);
    }

    void Application::createCamera3D(const char *name) {
        createCamera3D(name, {0.5, 0.5, 1});
    }

    void Application::createCamera3D(const char *name, const glm::vec3 &position) {
        auto camera3D = new engine::Camera3d {
            name,
            engine::ViewMatrix {
                "view",
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
        delete cameraController;
        cameraController = new Camera3dController(camera3D);
        _renderer->setCameraController(cameraController);
    }

}