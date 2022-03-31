//
// Created by mecha on 04.10.2021.
//

#include <FLLayer.h>

#include <core/FileExtensions.h>
#include <graphics/light/Light.h>
#include <graphics/camera/CameraShaderScript.h>
#include <graphics/material/MaterialShaderScript.h>
#include <graphics/light/LightShaderScript.h>
#include <graphics/GraphicsObject.h>
#include <scripting/ScriptBuilder.h>

#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>

#include <imgui/imgui.h>

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
        auto objShader = createRef<BaseShaderProgram>(
                ShaderProps {
                        "obj",
                        "v_obj.glsl",
                        "f_obj.glsl",
                        EDITOR_SHADERS_PATH
                },
                vObjShader,
                fObjShader
        );
        auto objRenderer = engine::createRef<engine::VIRenderer>(objShader);

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

        auto objFrame = engine::createRef<engine::FrameBuffer>();
        engine::FrameBufferFormat objFrameFormat;
        objFrameFormat.colorAttachments = {
                { engine::ColorFormat::RGBA8 }
        };
        objFrameFormat.depthStencilAttachment = {engine::DepthStencilFormat::DEPTH24STENCIL8 };
        objFrameFormat.width = app->getWindow()->getWidth();
        objFrameFormat.height = app->getWindow()->getHeight();
        objFrameFormat.samples = 1;
        objFrame->updateFormat(objFrameFormat);

        _objPreview = engine::createRef<engine::MeshLayout>(
                engine::ImageLayoutProps {"Object Preview"},
                objRenderer,
                objFrame,
                objCameraController
        );

        _objPreview->setEntity(objCamera);

        _scenePreviewCallback = new FLLayer::ScenePreviewCallback(*this);
        _imagePreviewCallback = new FLLayer::ImagePreviewCallback(*this);

        sceneViewport.setCallback(_scenePreviewCallback);
        sceneViewport.setDragDropCallback(this);
        _texturePreview.setCallback(_imagePreviewCallback);
        _assetBrowser->setCallback(this);

        createTest();
    }

    void FLLayer::createTest() {
        app->setSkybox(engine::SkyboxCube(
                "Skybox",
                app->activeScene.get(),
                CubeMapTextureComponent {
                        "skybox",
                        {
                                { "skybox/front.jpg", TextureFaceType::FRONT },
                                { "skybox/back.jpg", TextureFaceType::BACK },
                                { "skybox/left.jpg", TextureFaceType::LEFT },
                                { "skybox/right.jpg", TextureFaceType::RIGHT },
                                { "skybox/top.jpg", TextureFaceType::TOP },
                                { "skybox/bottom.jpg", TextureFaceType::BOTTOM },
                        }
                }
        ));

        FONTS.create(
                "assets/fonts/opensans/OpenSans-Bold.ttf",
                90,
                "assets/bitmaps/OpenSans-Bold.bmp",
                "assets/bitmaps/OpenSans-Bold.txt"
        );

        FONTS.create(
                "assets/fonts/roboto/Roboto-Bold.ttf",
                90,
                "assets/bitmaps/Roboto-Bold.bmp",
                "assets/bitmaps/Roboto-Bold.txt"
        );

        auto openSans = TextureComponent { "OpenSans-Bold.bmp", TextureType::TEXTURE_2D };
        GET_TEXTURE(openSans, "assets/bitmaps")
                .setParams({
                                   { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
                                   { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
                                   { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
                                   { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE }
                           });

        auto roboto = TextureComponent { "Roboto-Bold.bmp", TextureType::TEXTURE_2D };
        GET_TEXTURE(roboto, "assets/bitmaps")
                .setParams({
                                   { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
                                   { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
                                   { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
                                   { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE }
                           });

        Text2dView(
                "Text2D",
                app->activeScene.get(),
                Text2d {
                        "OpenSans-Bold",
                        "assets/fonts/opensans/OpenSans-Bold.ttf",
                        "OpenSans-Bold.bmp",
                        transform3d(
                                { 0, 0, 0 },
                                { 3, 3, 0 },
                                { 1, 1, 1 }
                        ),
                        { 1, 1, 1, 1 }
                },
                app->getAspectRatio()
        );

        Text3dView(
                "Text3D",
                app->activeScene.get(),
                Text3d {
                        "Roboto-Bold",
                        "assets/fonts/roboto/Roboto-Bold.ttf",
                        "Roboto-Bold.bmp",
                        transform3d(
                                { 0, 0, 0 },
                                { 3, 3, 0 },
                                { 1, 1, 1 }
                        ),
                        { 1, 1, 1, 1 }
                }
        );

        math::random(-10, 10, 5, [this](const uint32_t& i, const float& r) {
            Object3d(
                    app->activeScene.get(),
                    "Quad" + std::to_string(i),
                    transform3d(
                            { r, r, r },
                            { r, r, r },
                            { 1, 1, 1 }
                    ),
                    BatchQuad({
                                      QuadVertex {{ -0.5, -0.5, 1 }, {1, 1, 0, 1}},
                                      QuadVertex {{ 0.5, -0.5, 1 }, {1, 1, 0, 1}},
                                      QuadVertex {{ 0.5, 0.5, 1 }, {1, 1, 0, 1}},
                                      QuadVertex {{ -0.5, 0.5, 1 }, {1, 1, 0, 1}},
                              })
            );
        });
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
        activeSceneCameraController->bind(engine::KeyCode::Z, engine::ZoomType::ZOOM_IN);
        activeSceneCameraController->bind(engine::KeyCode::X, engine::ZoomType::ZOOM_OUT);
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
        objCameraController->bind(engine::KeyCode::Z, engine::ZoomType::ZOOM_IN);
        objCameraController->bind(engine::KeyCode::X, engine::ZoomType::ZOOM_OUT);
        objCameraController->setPosition({0, 0, -1});
        objCameraController->applyChanges();

        _sceneHierarchy.setCallback(this);

        app->eventController.onKeyPressedMap[KeyCode::D1] = { [this](KeyCode keyCode) { app->setSampleSize(1); } };
        app->eventController.onKeyPressedMap[KeyCode::D4] = { [this](KeyCode keyCode) { app->setSampleSize(4); } };
        app->eventController.onKeyPressedMap[KeyCode::F] = { [this](KeyCode keyCode) {
            if (app->input->isKeyPressed(engine::KeyCode::LeftControl)) {
                app->getWindow()->enableFullScreen();
            }
        }};
        app->eventController.onKeyPressedMap[KeyCode::Escape] = { [this](KeyCode keyCode) {
            app->getWindow()->disableFullScreen();
        }};
        app->eventController.onKeyPressedMap[KeyCode::L] = { [this](KeyCode keyCode) {
            // add script to entity
            Entity newEntity(copy(app->activeScene).get());
            addDLLScript(newEntity, "Test");
        }};
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
        sceneViewport.setId(app->activeScene->getTextureId());
    }

    void FLLayer::onKeyPressed(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyPressed(keyCode);
        _objPreview->onKeyPressed(keyCode);
        if (sceneViewport.isFocused()) {
            activeSceneCameraController->onKeyPressed(keyCode);
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
        auto texture = TextureComponent { fileName, TextureType::TEXTURE_2D };
        const uint32_t &textureId = GET_TEXTURE_ID(texture, RUNTIME_TEXTURES_PATH);
        _texturePreview.setId(textureId);
        _texturePreview.show();
    }

    void FLLayer::onObjOpen(const std::string &fileName) {
        EDITOR_INFO("onObjOpen({0})", fileName);
        const auto& objMesh = GET_OBJ(fileName);
        _objPreview->setMesh(objMesh);
        _objPreview->show();
    }

    void FLLayer::ImagePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        // do nothing.
    }

    void FLLayer::ScenePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        if (width == 0 || height == 0) return;

        _parent.app->activeSceneFrame->resize(width, height);
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
        app->activeSceneFrame->bind();
        setDepthTest(true);
        setClearColor({0.2, 0.2, 0.2, 1});
        clearDepthBuffer();

        app->activeSceneFrame->unbind();
        setDepthTest(false);
        clearColorBuffer();
    }

    void FLLayer::createAssetBrowser() {
        auto props = AssetBrowserProps {
                "Asset Browser",
                RUNTIME_ASSET_PATH
        };

        auto dirItem = AssetBrowserItem {
                "",
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "dir_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto pngItem = AssetBrowserItem {
                PNG_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "png_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto jpgItem = AssetBrowserItem {
                JPG_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "jpg_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto glslItem = AssetBrowserItem {
                GLSL_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "glsl_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto objItem = AssetBrowserItem {
                OBJ_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "obj_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto ttfItem = AssetBrowserItem {
                TTF_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "ttf_icon.png", EDITOR_TEXTURES_PATH)
        };

        auto cppItem = AssetBrowserItem {
                CPP_EXT,
                LOAD_TEXTURE_PARAMS(TextureType::TEXTURE_2D, "cpp_icon.png", EDITOR_TEXTURES_PATH)
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
        engine::Object3d {
                app->activeScene.get(),
                engine::FileSystem::getFileName(fileName),
                toMesh3dBatch(GET_OBJ(fileName))
        };
    }

    void FLLayer::onImageDragged(const std::string &fileName) {
        EDITOR_INFO("onImageDragged({0})", fileName);
    }
}