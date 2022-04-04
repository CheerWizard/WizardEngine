//
// Created by mecha on 04.10.2021.
//

#include <Activity.h>

#include <core/FileExtensions.h>
#include <graphics/light/Light.h>
#include <graphics/camera/CameraShaderScript.h>
#include <graphics/material/MaterialShaderScript.h>
#include <graphics/light/LightShaderScript.h>
#include <graphics/GraphicsObject.h>
#include <scripting/ScriptBuilder.h>

#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/core/geometry/Point.h>

#include <imgui/imgui.h>

#define BIND_KEY_PRESS(keycode, action) app->eventController.onKeyPressedMap[keycode] = { [this](KeyCode keyCode) { action }}

using namespace engine::shader;

namespace studio {

    void Activity::create() {
        createAssetBrowser();

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

        _scenePreviewCallback = new Activity::ScenePreviewCallback(*this);
        _imagePreviewCallback = new Activity::ImagePreviewCallback(*this);

        sceneViewport.setCallback(_scenePreviewCallback);
        sceneViewport.setDragDropCallback(this);
        screenViewport.setCallback(_scenePreviewCallback);
        screenViewport.setDragDropCallback(this);

        _texturePreview.setCallback(_imagePreviewCallback);
        _assetBrowser->setCallback(this);

        createTest();
    }

    void Activity::createTest() {
        auto scene1 = createRef<Scene>();
        app->scenes.emplace_back(scene1);

        activeSceneCamera = engine::Camera3D {
                "SceneCamera",
                app->getAspectRatio(),
                scene1.get()
        };

        activeSceneCameraController = engine::createRef<engine::Camera3dController>(
                "ActiveSceneCameraController",
                activeSceneCamera
        );

        scene1->setSkybox(engine::SkyboxCube(
                "Skybox",
                scene1.get(),
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

        auto points = Entity("Points", scene1.get());
        points.add<Points>(new PointVertex[4] {
                { { -0.5, 0.5 }, { 1, 0, 0 } },
                { { 0.5, 0.5 }, { 0, 1, 0 } },
                { { 0.5, -0.5 }, { 0, 0, 1 }},
                { { -0.5, -0.5 }, { 1, 1, 0 }}
        }, 4);

        Object3d(
                scene1.get(),
                "Sponza",
                transform3d(),
                GET_MESH_COMPONENT(BatchVertex<Vertex3d>, "assets/obj/sponza.obj")
        );

        Object3d(
                scene1.get(),
                "Quad",
                transform3d(),
                BatchQuad()
        );
    }

    void Activity::destroy() {
        delete _imagePreviewCallback;
        delete _scenePreviewCallback;
    }

    void Activity::onPrepare() {
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

        // switch between scenes : Scene0, Scene1
        BIND_KEY_PRESS(KeyCode::D0,
                       app->setActiveScene(0);
                        _sceneHierarchy.setScene(app->scenes[0]);
        );
        BIND_KEY_PRESS(KeyCode::D1,
                       app->setActiveScene(1);
                        _sceneHierarchy.setScene(app->scenes[1]);
        );
        // switch between fullscreen/windowed modes
        BIND_KEY_PRESS(KeyCode::F, if (app->input->isKeyPressed(engine::KeyCode::LeftControl)) {
            app->getWindow()->enableFullScreen();
        });
        BIND_KEY_PRESS(KeyCode::Escape, app->getWindow()->disableFullScreen(););
        BIND_KEY_PRESS(KeyCode::M, app->setSampleSize(8););
        BIND_KEY_PRESS(KeyCode::N, app->setSampleSize(1););

        setMSAA(true);
        app->getWindow()->loadGamepadMappings("assets/db/game_controller_db.txt");
    }

    void Activity::onRender(engine::Time dt) {
        sceneViewport.onUpdate(dt);
        screenViewport.onUpdate(dt);
        _sceneHierarchy.onUpdate(dt);
        _assetBrowser->onUpdate(dt);
        _texturePreview.onUpdate(dt);
        _objPreview->onUpdate(dt);
    }

    void Activity::onUpdate(engine::Time dt) {
        ImGuiLayer::onUpdate(dt);
        activeSceneCameraController->setDeltaTime(dt);
        sceneViewport.setId(app->screenFrame->getColorAttachment(0).id);
        screenViewport.setId(app->screenFrame->getColorAttachment(0).id);
    }

    void Activity::onKeyPressed(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyPressed(keyCode);
        _objPreview->onKeyPressed(keyCode);
        if (sceneViewport.isFocused() || screenViewport.isFocused()) {
            activeSceneCameraController->onKeyPressed(keyCode);
        }
    }

    void Activity::onKeyHold(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyHold(keyCode);
        _objPreview->onKeyHold(keyCode);
        if (sceneViewport.isFocused() || screenViewport.isFocused()) {
            activeSceneCameraController->onKeyHold(keyCode);
        }
    }

    void Activity::onKeyReleased(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyReleased(keyCode);
        _objPreview->onKeyReleased(keyCode);
        if (sceneViewport.isFocused() || screenViewport.isFocused()) {
            activeSceneCameraController->onKeyReleased(keyCode);
        }
    }

    void Activity::onKeyTyped(engine::KeyCode keyCode) {
        engine::ImGuiLayer::onKeyTyped(keyCode);
        _objPreview->onKeyTyped(keyCode);
        if (sceneViewport.isFocused() || screenViewport.isFocused()) {
            activeSceneCameraController->onKeyTyped(keyCode);
        }
    }

    void Activity::onMousePressed(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMousePressed(mouseCode);
        sceneViewport.onMousePressed(mouseCode);
        screenViewport.onMousePressed(mouseCode);
    }

    void Activity::onMouseRelease(engine::MouseCode mouseCode) {
        engine::ImGuiLayer::onMouseRelease(mouseCode);
        sceneViewport.onMouseRelease(mouseCode);
        screenViewport.onMouseRelease(mouseCode);
    }

    void Activity::onImageOpen(const std::string &fileName) {
        EDITOR_INFO("onImageOpen({0})", fileName);
        auto texture = TextureComponent { fileName, TextureType::TEXTURE_2D };
        const uint32_t &textureId = GET_TEXTURE_ID(texture, RUNTIME_TEXTURES_PATH);
        _texturePreview.setId(textureId);
        _texturePreview.show();
    }

    void Activity::onObjOpen(const std::string &fileName) {
        EDITOR_INFO("onObjOpen({0})", fileName);
        const auto& objMesh = GET_MESH_COMPONENT(io::ModelVertex, fileName);
        _objPreview->setMesh(objMesh);
        _objPreview->show();
    }

    void Activity::ImagePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        // do nothing.
    }

    void Activity::ScenePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        if (width == 0 || height == 0) return;

        _parent.app->activeSceneFrame->resize(width, height);
        _parent.app->screenFrame->resize(width, height);
        _parent.activeSceneCameraController->onWindowResized(width, height);
    }

    void Activity::onAssetImported(const std::string &assetPath) {
        EDITOR_INFO("onAssetImported() - {0}", assetPath);
    }

    void Activity::onAssetExported(const std::string &assetPath) {
        EDITOR_INFO("onAssetExported() - {0}", assetPath);
    }

    void Activity::onAssetRemoved(const std::string &assetPath) {
        EDITOR_INFO("onAssetRemoved() - {0}", assetPath);
    }

    void Activity::onEntityRemoved(const engine::Entity &entity) {
        EDITOR_INFO("onEntityRemoved({0})", entity.operator unsigned int());
        app->activeSceneFrame->bind();
        setDepthTest(true);
        setClearColor({0.2, 0.2, 0.2, 1});
        clearDepthBuffer();

        FrameBuffer::bindDefault();
        setDepthTest(false);
        clearColorBuffer();
    }

    void Activity::createAssetBrowser() {
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

    void Activity::onWindowClosed() {
        Layer::onWindowClosed();
        app->shutdown();
    }

    void Activity::onObjDragged(const std::string &fileName) {
        EDITOR_INFO("onObjDragged({0})", fileName);
        engine::Object3d {
                app->activeScene.get(),
                engine::FileSystem::getFileName(fileName),
                GET_MESH_COMPONENT(BatchVertex<Vertex3d>, fileName)
        };
    }

    void Activity::onImageDragged(const std::string &fileName) {
        EDITOR_INFO("onImageDragged({0})", fileName);
    }
}