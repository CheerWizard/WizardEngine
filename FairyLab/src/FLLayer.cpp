//
// Created by mecha on 04.10.2021.
//

#include "FLLayer.h"

#include "core/FileExtensions.h"
#include "graphics/light/Light.h"
#include "graphics/camera/CameraShaderScript.h"
#include "graphics/material/MaterialShaderScript.h"
#include "graphics/light/LightShaderScript.h"
#include "graphics/GraphicsObject.h"
#include "scripting/ScriptBuild.h"

#include <imgui/imgui/imgui.h>
#include "random"

using namespace engine::shader;

namespace fairy {

    void FLLayer::create() {
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

        auto vObjShader = BaseShader({
            camera3dScript()
        });
        auto fObjShader = BaseShader({
            materialScript(),
            phongLightScript()
        });
        auto objShader = BaseShaderProgram(
                ShaderProps {
                    "obj",
                    "v_obj.glsl",
                    "f_obj.glsl",
                    EDITOR_SHADERS_PATH
                },
                vObjShader,
                fObjShader
        );
        auto objRenderer = engine::createRef<engine::Renderer>(objShader);

        auto objCamera = engine::Camera3D {
            "obj",
            app->getAspectRatio(),
            editorScene.get()
        };
        auto objCameraController = engine::createRef<engine::Camera3dController>(
                "ObjCameraController",
                objCamera
        );

        auto objTransform = engine::transform3d(
                { 2.5, 0, 12 },
                {135, 0, 0},
                {0.5, 0.5, 0.5}
        );
        objCamera.add<engine::Transform3dComponent>(objTransform);

        auto objMaterial = engine::MaterialComponent();
        objMaterial.color.value = { 0.25, 0, 0, 1 };
        objCamera.add<engine::MaterialComponent>(objMaterial);

        auto objPhongLight = engine::PhongLightComponent();
        objPhongLight.position.value = { 25, 25, -25, 0 };
        objPhongLight.ambient.value = { 0.05, 0.05, 0.05, 0 };
        objPhongLight.diffuse.value = { 0.4, 0.4, 0.4, 0 };
        objPhongLight.specular.value = { 0.4, 0.4, 0.4, 0 };
        objCamera.add<engine::PhongLightComponent>(objPhongLight);

        auto objFrameController = engine::createRef<engine::FrameController>();
        objFrameController->updateSpecs(app->getWindowWidth(), app->getWindowHeight());

        _objPreview = engine::createRef<engine::MeshLayout>(
                engine::ImageLayoutProps {"Object Preview"},
                objRenderer,
                objFrameController,
                objCameraController
        );

        _objPreview->setEntity(objCamera);

        _scenePreviewCallback = new FLLayer::ScenePreviewCallback(*this);
        _imagePreviewCallback = new FLLayer::ImagePreviewCallback(*this);

        sceneViewport.setCallback(_scenePreviewCallback);
        sceneViewport.setDragDropCallback(this);
        _texturePreview.setCallback(_imagePreviewCallback);
        _assetBrowser->setCallback(this);

//        app->getTextureSource()->loadTexture("demo.png");
//        app->getTextureSource()->loadTexture("demo_texture.jpg");

        auto car = engine::Object3d(
                app->activeScene.get(),
                "Car",
                engine::transform3d(
                        { 2.5, 0, 12 },
                        {135, 0, 0},
                        {0.5, 0.5, 0.5}
                ),
                GET_MESH("ferrari.obj")
        );

//        _cubeEntity = engine::Entity("Cube", app->activeScene.get());
//        auto cubeMesh = app->getMeshSource()->getCube("cube");
//        auto cubeMaterial = engine::MaterialComponent();
//        auto cubeMaterialMaps = engine::MaterialMapsComponent();
//        cubeMaterialMaps.diffuseFileName = "wood_diffuse.png";
//        cubeMaterialMaps.specularFileName = "wood_specular.png";
//        _cubeEntity.add<engine::Transform3dComponent>(cubeTransform);
//        _cubeEntity.add<engine::MeshComponent>(cubeMesh);
//        _cubeEntity.add<engine::MaterialComponent>(cubeMaterial);
//        _cubeEntity.add<engine::MaterialMapsComponent>(cubeMaterialMaps);

//        auto sphereFamily = engine::Family("Cars");
//        auto sphereMesh = app->getMeshSource()->getMesh("ferrari.obj");
//        sphereFamily.add<engine::MeshComponent>(sphereMesh);
//
//        // randomize 100 spheres. testing Instance Rendering and Family approach!
//        std::random_device rd;
//        std::mt19937 mt(rd());
//        std::uniform_real_distribution<double> dist(-100, 100);
//        for (uint32_t i = 0 ; i < 10 ; i++) {
//            auto r = (float) dist(mt);
//
//            auto sphereEntity = engine::Entity("Ferrari" + std::to_string(i), &sphereFamily);
//            auto sphereTransform = engine::TransformComponents::newTransform3d(
//                    { r, r, r },
//                    { r, r, r },
//                    { 0.2, 0.2, 0.2 }
//            );
//            auto sphereMaterial = engine::MaterialComponent();
//
//            sphereEntity.add<engine::Transform3dComponent>(sphereTransform);
//            sphereEntity.add<engine::MaterialComponent>(sphereMaterial);
//        }
//        app->activeScene->addFamily(sphereFamily);

        // light
//        auto phongLight = engine::PhongLight(app->activeScene.get());
//        auto directLight = engine::DirectLight(app->activeScene.get());
//        auto pointLight = engine::PointLight(app->activeScene.get());
//        auto lightMesh = engine::Shapes::newSquare();
        auto lightTransform = engine::transform3d(
                { 10, 0, 12 },
                { 135, 0, 0 },
                { 0.5, 0.5, 0.5 }
        );
        auto phongLight = engine::PhongLight(app->activeScene.get());
        phongLight.add<engine::Transform3dComponent>(lightTransform);
//        phongLight.add<engine::MeshComponent>(lightMesh);
//
        auto pointLight = engine::PointLight(app->activeScene.get());
        pointLight.add<engine::Transform3dComponent>(lightTransform);

        auto pointLight2 = engine::PointLight(app->activeScene.get());
        pointLight2.add<engine::Transform3dComponent>(lightTransform);

        auto pointLight3 = engine::PointLight(app->activeScene.get());
        pointLight3.add<engine::Transform3dComponent>(lightTransform);

        auto pointLight4 = engine::PointLight(app->activeScene.get());
        pointLight4.add<engine::Transform3dComponent>(lightTransform);

//        pointLight.add<engine::MeshComponent>(engine::copy(lightMesh));

//        auto sponzaEntity = engine::Entity("Sponza", app->activeScene.get());
//        auto sponzaMesh = app->getMeshSource()->getMesh("sponza.obj");
//        auto sponzaTransform = engine::TransformComponents::newTransform3d(
//                { 10, 0, 12 },
//                { 135, 0, 0 },
//                { 0.5, 0.5, 0.5 }
//        );
//        sponzaEntity.add<engine::Transform3dComponent>(sponzaTransform);
//        sponzaEntity.add<engine::MeshComponent>(sponzaMesh);
        auto humanMaterialMaps = engine::MaterialMapsComponent();
        humanMaterialMaps.diffuseFileName = "wood_diffuse.png";
        humanMaterialMaps.specularFileName = "wood_specular.png";
        car.add<engine::MaterialMapsComponent>(humanMaterialMaps);
        // init script dll
        ScriptBuild::init({ "Engine", "Runtime" });
        // add script to entity
        ScriptBuild::addDLLScript(car);
    }

    void FLLayer::destroy() {
        delete _imagePreviewCallback;
        delete _scenePreviewCallback;
    }

    void FLLayer::onPrepare() {
        EDITOR_INFO("onPrepare()");
        ImGuiLayer::onPrepare();
        activeSceneCameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        activeSceneCameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        activeSceneCameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        activeSceneCameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        activeSceneCameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        activeSceneCameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
//        activeSceneCameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
//        activeSceneCameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        activeSceneCameraController->setPosition({0, 0, -1});
        activeSceneCameraController->applyChanges();

        _texturePreview.hide();
        _objPreview->hide();

        const auto& objCameraController = _objPreview->getCameraController();
        objCameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        objCameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        objCameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        objCameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        objCameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        objCameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
//        objCameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
//        objCameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        objCameraController->setPosition({0, 0, -1});
        objCameraController->applyChanges();

        _sceneHierarchy.setCallback(this);
    }

    void FLLayer::onRender(engine::Time dt) {
        sceneViewport.onUpdate(dt);
        _sceneHierarchy.onUpdate(dt);
        _assetBrowser->onUpdate(dt);
        _texturePreview.onUpdate(dt);
        _objPreview->onUpdate(dt);
    }

    void FLLayer::onUpdate(engine::Time dt) {
        ImGuiLayer::onUpdate(dt);
        activeSceneCameraController->setDeltaTime(dt);
        // update scene texture id!
        sceneViewport.setId(app->activeScene->getTextureId());
    }

    void FLLayer::onKeyPressed(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyPressed(keyCode);

        _objPreview->onKeyPressed(keyCode);

        if (sceneViewport.isFocused()) {
            activeSceneCameraController->onKeyPressed(keyCode);
        }

        // L-CTRL + F - enable fullscreen mode.
        if (keyCode == engine::KeyCode::F && app->input->isKeyPressed(engine::KeyCode::LeftControl)) {
            app->getWindow()->enableFullScreen();
        }

        // ESC - exit fullscreen mode.
        if (keyCode == engine::Escape) {
            app->getWindow()->disableFullScreen();
        }
    }

    void FLLayer::onKeyHold(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyHold(keyCode);
        _objPreview->onKeyHold(keyCode);

        if (sceneViewport.isFocused()) {
            activeSceneCameraController->onKeyHold(keyCode);
        }
    }

    void FLLayer::onKeyReleased(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyReleased(keyCode);
        _objPreview->onKeyReleased(keyCode);

        if (sceneViewport.isFocused()) {
            activeSceneCameraController->onKeyReleased(keyCode);
        }
    }

    void FLLayer::onKeyTyped(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyTyped(keyCode);
        _objPreview->onKeyTyped(keyCode);

        if (sceneViewport.isFocused()) {
            activeSceneCameraController->onKeyTyped(keyCode);
        }
    }

    void FLLayer::onMousePressed(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMousePressed(mouseCode);
        sceneViewport.onMousePressed(mouseCode);
    }

    void FLLayer::onMouseRelease(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMouseRelease(mouseCode);
        sceneViewport.onMouseRelease(mouseCode);
    }

    void FLLayer::onImageOpen(const std::string &fileName) {
        EDITOR_INFO("onImageOpen({0})", fileName);
        const uint32_t &textureId = GET_TEXTURE_ID(fileName);
        _texturePreview.setId(textureId);
        _texturePreview.show();
    }

    void FLLayer::onObjOpen(const std::string &fileName) {
        EDITOR_INFO("onObjOpen({0})", fileName);
        const auto& objMesh = GET_MESH(fileName);
        _objPreview->setMesh(objMesh);
        _objPreview->show();
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
        EDITOR_INFO("onAssetImported() - {0}", assetPath);
    }

    void FLLayer::onAssetExported(const std::string &assetPath) {
        EDITOR_INFO("onAssetExported() - {0}", assetPath);
    }

    void FLLayer::onAssetRemoved(const std::string &assetPath) {
        EDITOR_INFO("onAssetRemoved() - {0}", assetPath);
    }

    void FLLayer::onEntityRemoved(const engine::Entity &entity) {
        EDITOR_INFO("onEntityRemoved({0})", entity.operator unsigned int());
        app->activeFrameController->resetFrame();
    }

    void FLLayer::createAssetBrowser() {
        auto props = AssetBrowserProps {
                "Asset Browser",
                RUNTIME_ASSET_PATH
        };

        auto dirItem = AssetBrowserItem {
            "",
            LOAD_TEXTURE("dir_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto pngItem = AssetBrowserItem {
            PNG_EXT,
            LOAD_TEXTURE("png_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto jpgItem = AssetBrowserItem {
            JPG_EXT,
            LOAD_TEXTURE("jpg_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto glslItem = AssetBrowserItem {
            GLSL_EXT,
            LOAD_TEXTURE("glsl_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto objItem = AssetBrowserItem {
            OBJ_EXT,
            LOAD_TEXTURE("obj_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto ttfItem = AssetBrowserItem {
            TTF_EXT,
            LOAD_TEXTURE("ttf_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto cppItem = AssetBrowserItem {
            CPP_EXT,
            LOAD_TEXTURE("cpp_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto items = AssetBrowserItems<6> {
            dirItem,
            { pngItem, jpgItem, glslItem, objItem, ttfItem, cppItem }
        };

        auto fileDialog = app->createFileDialog();

        _assetBrowser = engine::createRef<AssetBrowser>(props, items, fileDialog);
    }

    void FLLayer::onWindowClosed() {
        Layer::onWindowClosed();
        app->shutdown();
    }

    void FLLayer::onObjDragged(const std::string &fileName) {
        EDITOR_INFO("onObjDragged({0})", fileName);
        auto newObject3d = engine::Object3d { app->activeScene.get(), engine::FileSystem::getFileName(fileName), GET_MESH(fileName) };
        // this function is required to reload objects into video memory!
        // otherwise, they will not be displayed, until mesh and transform will not be changed!
        engine::updateObjects3d(app->activeScene);
    }

    void FLLayer::onImageDragged(const std::string &fileName) {
        EDITOR_INFO("onImageDragged({0})", fileName);
    }

}