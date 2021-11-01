//
// Created by mecha on 04.10.2021.
//

#include "FLLayer.h"
#include "core/FileExtensions.h"

#include <imgui/imgui/imgui.h>

#include "random"

namespace fairy {

    void FLLayer::create() {
        ImGui::StyleColorsLight();

        createAssetBrowser();

        activeSceneCamera = engine::Camera3D {
            "SceneCamera",
            app->getAspectRatio(),
            app->activeScene.get()
        };
        activeSceneCameraController = engine::createRef<engine::Camera3dController>(
                "ActiveSceneCameraController",
                activeSceneCamera
        );
        app->activeScene->addEntity(activeSceneCamera);

        const auto& graphicsFactory = app->getGraphicsFactory();

        auto objPreviewShader = app->getShaderSource()->create(engine::ShaderProps {
            "obj",
            "v_obj",
            "f_obj",
            EDITOR_SHADERS_PATH
        });
        auto objRenderer = engine::createRef<engine::Renderer>(
                graphicsFactory,
                objPreviewShader,
                app->getTextureSource()
        );

        auto objCamera = engine::Camera3D {
            "obj",
            app->getAspectRatio(),
            editorScene.get()
        };
        auto objCameraController = engine::createRef<engine::Camera3dController>(
                "ObjCameraController",
                objCamera
        );

        auto objTexture = engine::TextureComponent {
            "demo.png",
            "diffuseSampler",
            0
        };
        objCamera.add<engine::TextureComponent>(objTexture);
        editorScene->addEntity(objCamera);

        auto objFrameController = engine::createRef<engine::FrameController>(graphicsFactory->newFrameBuffer());
        objFrameController->updateSpecs(app->getWindowWidth(), app->getWindowHeight());

        _objPreview = engine::createRef<engine::MeshLayout>(
                engine::ImageLayoutProps {"Object Preview"},
                objRenderer,
                objFrameController,
                objCameraController
        );

        auto objTransform = engine::TransformComponents::newTransform3d(
                { 2.5, 0, 12 },
                {135, 0, 0},
                {0.5, 0.5, 0.5}
        );
        objCamera.add<engine::Transform3dComponent>(objTransform);

        _objPreview->setEntity(objCamera);

        _scenePreviewCallback = new FLLayer::ScenePreviewCallback(*this);
        _imagePreviewCallback = new FLLayer::ImagePreviewCallback(*this);

        _scenePreview.setCallback(_scenePreviewCallback);
        _texturePreview.setCallback(_imagePreviewCallback);
        _assetBrowser->setCallback(this);

        app->getTextureSource()->loadTexture("demo.png");
        app->getTextureSource()->loadTexture("demo_texture.jpg");

        _humanEntity = app->activeScene->createEntity("Human");
        auto humanMesh = app->getMeshSource().getMesh("human.obj");
        auto humanTransform = engine::TransformComponents::newTransform3d(
                { 2.5, 0, 12 },
                {135, 0, 0},
                {0.5, 0.5, 0.5}
        );
        auto humanTexture = engine::TextureComponent {
            "demo.png",
            "diffuseSampler",
            0
        };
        _humanEntity.add<engine::Transform3dComponent>(humanTransform);
        _humanEntity.add<engine::MeshComponent>(humanMesh);
        _humanEntity.add<engine::TextureComponent>(humanTexture);

        _cubeEntity = app->activeScene->createEntity("Cube");
        auto cubeMesh = app->getMeshSource().getCube("cube");
        auto cubeTransform = engine::TransformComponents::newTransform3d(
                { 10, 0, 12 },
                { 135, 0, 0 },
                { 0.5, 0.5, 0.5 }
        );
        auto cubeTexture = engine::TextureComponent {
            "demo_texture.jpg",
            "diffuseSampler",
            1
        };
        _cubeEntity.add<engine::Transform3dComponent>(cubeTransform);
        _cubeEntity.add<engine::MeshComponent>(cubeMesh);
        _cubeEntity.add<engine::TextureComponent>(cubeTexture);

        auto sphereFamily = engine::Family("Spheres", app->activeScene.get());
        auto sphereMesh = app->getMeshSource().getMesh("sphere.obj");
        sphereFamily.add<engine::MeshComponent>(sphereMesh);

        sphereFamily.addEntity(activeSceneCamera);

        // randomize 100 spheres. testing Instance Rendering and Family approach!
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(-10, 10.0);
        for (uint32_t i = 0 ; i < 100 ; i++) {
            auto r = (float) dist(mt);

            auto sphereEntity = engine::Entity("Sphere" + std::to_string(i), app->activeScene.get());
            auto sphereTransform = engine::TransformComponents::newTransform3d(
                    { r, r, r },
                    { r, r, r },
                    { r, r, r }
            );
            auto sphereTexture = engine::TextureComponent {
                "demo.png",
                "diffuseSampler",
                0
            };

            sphereEntity.add<engine::Transform3dComponent>(sphereTransform);
            sphereEntity.add<engine::TextureComponent>(sphereTexture);

            sphereFamily.addEntity(sphereEntity);
        }

        app->activeScene->addFamily(sphereFamily);
    }

    void FLLayer::destroy() {
        _scenePreview.removeCallback();
        _texturePreview.removeCallback();
        delete _imagePreviewCallback;
        delete _scenePreviewCallback;
    }

    void FLLayer::onPrepare() {
        CLIENT_INFO("onPrepare()");
        ImGuiLayer::onPrepare();
        app->bindCloseKey(engine::KeyCode::Escape);
        activeSceneCameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        activeSceneCameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        activeSceneCameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        activeSceneCameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        activeSceneCameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        activeSceneCameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
        activeSceneCameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
        activeSceneCameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        activeSceneCameraController->setPosition({0, 0, -1});
        activeSceneCameraController->applyChanges();

        _scenePreview.setId(app->activeScene->getTextureId());

        _texturePreview.hide();
        _objPreview->hide();

        const auto& objCameraController = _objPreview->getCameraController();
        objCameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        objCameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        objCameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        objCameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        objCameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        objCameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
        objCameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
        objCameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        objCameraController->setPosition({0, 0, -1});
        objCameraController->applyChanges();

        _fileEditor.setTextFieldFont(resizableFont);
        _sceneHierarchy.setCallback(this);
    }

    void FLLayer::onRender(engine::Time dt) {
        _scenePreview.onUpdate(dt);
        _sceneHierarchy.onUpdate(dt);
        _assetBrowser->onUpdate(dt);
        _texturePreview.onUpdate(dt);
        _objPreview->onUpdate(dt);
        _fileEditor.onUpdate(dt);
    }

    void FLLayer::onUpdate(engine::Time dt) {
        ImGuiLayer::onUpdate(dt);
        activeSceneCameraController->setDeltaTime(dt);
    }

    void FLLayer::onKeyPressed(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyPressed(keyCode);

        _objPreview->onKeyPressed(keyCode);

        if (_scenePreview.isFocused()) {
            activeSceneCameraController->onKeyPressed(keyCode);
        }

        // L-CTRL + F - toggles fullscreen/windowed modes.
        if (keyCode == engine::KeyCode::F && app->input->isKeyPressed(engine::KeyCode::LeftControl)) {
            app->getWindow()->toggleFullScreen();
        }

        // L-CTRL + S - saves file changes in FileEditor if it's visible.
        if (keyCode == engine::KeyCode::S && app->input->isKeyPressed(engine::KeyCode::LeftControl)) {
            if (_fileEditor.props.isVisible) {
                _fileEditor.save();
            }
        }
    }

    void FLLayer::onKeyHold(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyHold(keyCode);
        _objPreview->onKeyHold(keyCode);

        if (_scenePreview.isFocused()) {
            activeSceneCameraController->onKeyHold(keyCode);
        }
    }

    void FLLayer::onKeyReleased(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyReleased(keyCode);
        _objPreview->onKeyReleased(keyCode);

        if (_scenePreview.isFocused()) {
            activeSceneCameraController->onKeyReleased(keyCode);
        }
    }

    void FLLayer::onKeyTyped(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyTyped(keyCode);
        _objPreview->onKeyTyped(keyCode);

        if (_scenePreview.isFocused()) {
            activeSceneCameraController->onKeyTyped(keyCode);
        }
    }

    void FLLayer::onMousePressed(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMousePressed(mouseCode);
        _scenePreview.onMousePressed(mouseCode);
    }

    void FLLayer::onMouseRelease(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMouseRelease(mouseCode);
        _scenePreview.onMouseRelease(mouseCode);
    }

    void FLLayer::onImageOpen(const std::string &fileName) {
        ENGINE_INFO("onImageOpen({0})", fileName);
        const uint32_t &textureId = app->getTextureSource()->getTextureBuffer(fileName)->getId();
        _texturePreview.setId(textureId);
        _texturePreview.show();
    }

    void FLLayer::onObjOpen(const std::string &fileName) {
        ENGINE_INFO("onObjOpen({0})", fileName);
        const auto& objMesh = app->getMeshSource().getMesh(fileName);
        _objPreview->setMesh(objMesh);
        _objPreview->show();
    }

    void FLLayer::onGlslOpen(const std::string &filePath, const std::string &fileName) {
        ENGINE_INFO("onGlslOpen({0})", filePath);
        _fileEditor.props.title = filePath;
        _fileEditor.open(filePath);
        _fileEditor.show();
    }

    void FLLayer::ImagePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        // do nothing.
    }

    void FLLayer::ScenePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        if (width == 0 || height == 0) return;

        _parent.app->activeFrameController->resize(width, height);
        _parent.activeSceneCameraController->onWindowResized(width, height);
    }

    void FLLayer::onAssetImported(const std::string &assetPath) {
        ENGINE_INFO("onAssetImported() - {0}", assetPath);
    }

    void FLLayer::onAssetExported(const std::string &assetPath) {
        ENGINE_INFO("onAssetExported() - {0}", assetPath);
    }

    void FLLayer::onAssetRemoved(const std::string &assetPath) {
        ENGINE_INFO("onAssetRemoved() - {0}", assetPath);
    }

    void FLLayer::onEntityRemoved(const engine::Entity &entity) {
        ENGINE_INFO("onEntityRemoved({0})", entity.operator unsigned int());
        app->activeFrameController->resetFrame();
    }

    void FLLayer::createAssetBrowser() {
        const auto& textureSource = app->getTextureSource();

        auto props = AssetBrowserProps {
            "Asset Browser",
            CLIENT_ASSET_PATH
        };

        auto dirItem = AssetBrowserItem {
            "",
            textureSource->loadTexture("dir_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto pngItem = AssetBrowserItem {
            engine::file_extensions::PNG,
            textureSource->loadTexture("png_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto jpgItem = AssetBrowserItem {
            engine::file_extensions::JPG,
            textureSource->loadTexture("jpg_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto glslItem = AssetBrowserItem {
            engine::file_extensions::GLSL,
            textureSource->loadTexture("glsl_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto objItem = AssetBrowserItem {
            engine::file_extensions::OBJ,
            textureSource->loadTexture("obj_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto ttfItem = AssetBrowserItem {
            engine::file_extensions::TTF,
            textureSource->loadTexture("ttf_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto items = AssetBrowserItems<AssetBrowser::itemsCount> {
            dirItem,
            { pngItem, jpgItem, glslItem, objItem, ttfItem }
        };

        auto fileDialog = app->createFileDialog();

        _assetBrowser = engine::createRef<AssetBrowser>(props, items, fileDialog);
    }

}