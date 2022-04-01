//
// Created by mecha on 04.10.2021.
//

#include <FLApplication.h>
#include <FLLayer.h>
#include <Launcher.h>

#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/GraphicsObject.h>

engine::Application* engine::createApplication() {
    EDITOR_INFO("Creating FLApplication...");
    return new fairy::FLApplication();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Runtime";
}

namespace fairy {

    void FLApplication::onCreate() {
        Application::onCreate();
        EDITOR_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);
        setWindowIcon("FairyLab.png");

        auto* flLayer = new FLLayer(
                this,
                engine::ImGuiLayerProps {
                    getWindowWidth(),
                    getWindowHeight()
                }
        );

        pushBack(flLayer);

//        createTest();
    }

    engine::WindowProps FLApplication::createWindowProps() {
        return engine::WindowProps {
            "Fairy Lab"
        };
    }

    void FLApplication::createTest() {
        auto scene1 = activeScene;

        engine::Camera3D {
                "SceneCamera",
                getAspectRatio(),
                scene1.get()
        };

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
                scene1.get(),
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
                getAspectRatio()
        );

        Text3dView(
                "Text3D",
                scene1.get(),
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

        math::random(-10, 10, 5, [&scene1](const uint32_t& i, const float& r) {
            Object3d(
                    scene1.get(),
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
}