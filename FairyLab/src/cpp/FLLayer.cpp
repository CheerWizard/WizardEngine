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

#include <graphics/core/geometry/Line.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/core/geometry/Circle.h>
#include <graphics/outline/Outline.h>
#include <graphics/skybox/Skybox.h>

#include <core/Fonts.h>

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
        objFrame->updateSpecs(app->getWindowWidth(), app->getWindowHeight());

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

//        app->getTextureSource()->loadTexture("demo.png");
//        app->getTextureSource()->loadTexture("demo_texture.jpg");

        auto carMesh = toMesh3dBatch(GET_OBJ("ferrari.obj"));
        auto carTransform = engine::transform3d(
                { 2.5, 0, 12 },
                {135, 0, 0},
                {0.5, 0.5, 0.5}
        );
        car = engine::Object3d(
                app->activeScene.get(),
                "Car",
                carTransform,
                carMesh
        );
        car.add<OutlineBatchMesh>(toOutlineBatchMesh(copy(carMesh)));
        car.add<OutlineComponent>(OutlineComponent({ 1, 1, 0, 1 }, 0.01));

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

        auto humanMaterialMaps = engine::MaterialMapsComponent();
        humanMaterialMaps.diffuse.fileName = "wood_diffuse.png";
        humanMaterialMaps.specular.fileName = "wood_specular.png";

        random(-10, 10, 10, [this](const uint32_t& i, const float& r) {
            auto humanMesh = toMesh3dBatch(GET_OBJ("human.obj"));
            humanMesh = copy(humanMesh);
            auto human = engine::Object3d(
                app->activeScene.get(),
                "Human " + std::to_string(i),
                engine::transform3d(
                        { r, r, r },
                        { r, r, r },
                        {0.2, 0.2, 0.2}
                ),
                humanMesh
            );
            human.add<OutlineBatchMesh>(toOutlineBatchMesh(copy(humanMesh)));
            human.add<OutlineComponent>(OutlineComponent({0, 1, 0, 1}, 0.01));
        });

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
        auto carMaterialMaps = engine::MaterialMapsComponent();
        carMaterialMaps.diffuse.fileName = "wood_diffuse.png";
        carMaterialMaps.specular.fileName = "wood_specular.png";
        car.add<engine::MaterialMapsComponent>(carMaterialMaps);

        random(-10, 10, 10, [this](const uint32_t& i, const float& r) {
            glm::vec4 randomColor1 = { random(0, 1), random(0, 1), random(0, 1), 1 };
            glm::vec4 randomColor2 = { random(0, 1), random(0, 1), random(0, 1), 1 };
            glm::vec3 randomPos1 = { random(-1, 1), random(-1, 1), random(-1, 1) };
            glm::vec3 randomPos2 = { random(-1, 1), random(-1, 1), random(-1, 1) };
            engine::Object3d(
                    app->activeScene.get(),
                    "Line " + std::to_string(i),
                    engine::transform3d(
                        { r, r, r },
                        { r, r, r },
                        { 1, 1, 1 }
                    ),
                    InstanceLine({
                        { randomPos1, randomColor1 },
                        { randomPos2, randomColor2 }
                    })
            );
        });

        random(-10, 10, 10, [this](const uint32_t& i, const float& r) {
            glm::vec4 randomColor1 = { random(0, 1), random(0, 1), random(0, 1), 1 };
            glm::vec4 randomColor2 = { random(0, 1), random(0, 1), random(0, 1), 1 };
            glm::vec4 randomColor3 = { random(0, 1), random(0, 1), random(0, 1), 1 };
            glm::vec4 randomColor4 = { random(0, 1), random(0, 1), random(0, 1), 1 };

            std::array<QuadVertex, 4> quadVertices = {
                    QuadVertex { { 0.5, -0.5, 0.5 }, randomColor1 },
                    QuadVertex { { 0.5, 0.5, 0.5 }, randomColor2 },
                    QuadVertex { { -0.5, -0.5, 0.5 }, randomColor3 },
                    QuadVertex { { -0.5, 0.5, 0.5 }, randomColor4 }
            };
            auto quadVertexData = InstanceQuad(quadVertices);
            auto quad = engine::Object3d(
                    app->activeScene.get(),
                    "Quad " + std::to_string(i),
                    engine::transform3d(
                            { r, r, r },
                            { r, r, r },
                            { 1, 1, 1 }
                    ),
                    quadVertexData
            );
//            quad.add<OutlineInstanceVertexData>(toOutlineInstanceVertexData(copy(quadVertexData)));
//            quad.add<OutlineComponent>(OutlineComponent({0, 1, 0, 1}, 0.01));
        });

        random(-10, 10, 10, [this](const uint32_t& i, const float& r) {
            glm::vec4 randomColor = { random(0, 1), random(0, 1), random(0, 1), 1 };
            std::array<CircleVertex, 4> circleVertices = {
                    CircleVertex { { 0.5, -0.5, 0.5 }, { 1, -1 } },
                    CircleVertex { { 0.5, 0.5, 0.5 }, { 1, 1 } },
                    CircleVertex { { -0.5, -0.5, 0.5 }, { -1, -1 } },
                    CircleVertex { { -0.5, 0.5, 0.5 }, { -1, 1 } }
            };

            auto circle = engine::Object3d(
                    app->activeScene.get(),
                    "Circle " + std::to_string(i),
                    engine::transform3d(
                            { r, r, r },
                            { r, r, r },
                            { 1, 1, 1 }
                    ),
                    InstanceCircle(circleVertices)
            );
            circle.add<CircleComponent>(CircleComponent(
                    randomColor,
                    random(0.0, 0.1),
                    random(0.0, 0.1)
            ));
        });

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

        FONTS_GENERATE_BITMAP("assets/fonts/opensans/OpenSans-Bold.ttf",
                              14,
                              "assets/bitmaps/OpenSans-Bold.bmp",
                              "assets/bitmaps/OpenSans-Bold.txt");
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
        // draw scene texture id!
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

        if (keyCode == engine::L) {
            // add script to entity
            Entity newEntity(copy(app->activeScene).get());
            addDLLScript(newEntity, "Test");
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

        _parent.app->activeFrame->resize(width, height);
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
        app->activeFrame->bind();
        setDepthTest(true);
        setClearColor({0.2, 0.2, 0.2, 1});
        clearDepthBuffer();

        app->activeFrame->unbind();
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