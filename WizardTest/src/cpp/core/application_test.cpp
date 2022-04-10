//
// Created by mecha on 05.04.2022.
//

#include <core/application_test.h>
#include <graphics/core/geometry/Point.h>
#include <graphics/camera/Camera.h>
#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/GraphicsObject.h>

using namespace engine::core;
using namespace engine::graphics;

Application* core::createApplication() {
    RUNTIME_INFO("Creating Test Application...");
    return new test::Application();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Test";
}

namespace test {

    void Application::onCreate() {
        core::Application::onCreate();
        createTest();
    }

    WindowProps Application::createWindowProps() {
        return WindowProps {
            "Test"
        };
    }

    void Application::createTest() {
        auto scene = createRef<Scene>();
        setActiveScene(scene);

        Camera3D {
                "SceneCamera",
                getAspectRatio(),
                scene.get()
        };

        u32 skyboxId = TextureBuffer::load(
                {
                        { "assets/textures/skybox/front.jpg", TextureFaceType::FRONT },
                        { "assets/textures/skybox/back.jpg", TextureFaceType::BACK },
                        { "assets/textures/skybox/left.jpg", TextureFaceType::LEFT },
                        { "assets/textures/skybox/right.jpg", TextureFaceType::RIGHT },
                        { "assets/textures/skybox/top.jpg", TextureFaceType::TOP },
                        { "assets/textures/skybox/bottom.jpg", TextureFaceType::BOTTOM },
                }
        );
        TextureBuffer::setDefaultParamsCubeMap(skyboxId);

        scene->setSkybox(SkyboxCube(
                "Skybox",
                scene.get(),
                CubeMapTextureComponent(skyboxId, TextureBuffer::getTypeId(TextureType::CUBE_MAP))
        ));

        auto pointsEntity = Entity("Points", scene.get());
        pointsEntity.add<Points>(Points { new PointVertex[4] {
                { { -0.5, 0.5 }, { 1, 0, 0 } },
                { { 0.5, 0.5 }, { 0, 1, 0 } },
                { { 0.5, -0.5 }, { 0, 0, 1 }},
                { { -0.5, -0.5 }, { 1, 1, 0 }}
        }, 4 });

        Object3d(scene.get(), "Quad", Transform3dComponent(), BatchQuad());
    }
}