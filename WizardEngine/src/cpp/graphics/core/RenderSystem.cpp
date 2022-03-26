//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>
#include <graphics/camera/CameraShaderScript.h>
#include <graphics/material/MaterialShaderScript.h>
#include <graphics/light/LightShaderScript.h>

#include <graphics/core/geometry/Line.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/core/geometry/Circle.h>
#include <graphics/outline/Outline.h>
#include <graphics/skybox/Skybox.h>

namespace engine {

    void RenderSystem::create() {
        createSceneRenderer();
        createLineRenderers();
        createQuadRenderer();
        createCircleRenderer();
        createOutlineRenderer();
        createSkyboxRenderer();
    }

    void RenderSystem::onUpdate() {
        if (activeScene->isEmpty()) return;

        sceneFrame->bind();

        // enables transparency
        setBlendMode(true);
        setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        setClearColor({0.2, 0.2, 0.2, 1});
        setDepthTest(true);
        setStencilTest(true);
        setStencilTestActions({ KEEP, KEEP, REPLACE });
        clearStencilBuffer();
        setStencilTestOperator(ALWAYS, 1, false);
        stencilMask(false);

        auto& registry = activeScene->getRegistry();
        // skybox
        skyboxRenderer->render<Transform3dComponent, SkyboxVertex>(skybox);
        // scene
        sceneRenderer->renderVI<Transform3dComponent, Vertex3d>(registry);
        // lines
        lineRenderer->renderV<Transform3dComponent, LineVertex>(registry);
        stripLineRenderer->renderV<Transform3dComponent, LineVertex>(registry);
        loopLineRenderer->renderV<Transform3dComponent, LineVertex>(registry);
        // quads
        quadRenderer->renderV<Transform3dComponent, QuadVertex>(registry);
        // circles
        circleRenderer->renderV<Transform3dComponent, CircleVertex>(registry);
        // stop write to stencil buffer
        setStencilTestOperator(NOT_EQUAL, 1, false);
        stencilMask(true);
        setDepthTest(false);
        // outlining
        outlineSceneRenderer->render<Transform3dComponent, OutlineVertex>(registry);

        // write to stencil buffer
        stencilMask(false);
        setStencilTestOperator(ALWAYS, 0, false);
        setDepthTest(true);

        sceneFrame->unbind();
        setDepthTest(false);
        clearColorBuffer();

        activeScene->updateComponents<Transform3dComponent>([](Transform3dComponent& transform) {
            transform.isUpdated = false;
        });
    }

    void RenderSystem::createSceneRenderer() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader({ materialScript(), phongLightScript(), materialMapScript(), pointLightArrayScript() });
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
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader({ materialScript(), phongLightScript(), materialMapScript(), pointLightArrayScript() });
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
        sceneRenderer = createRef<MultiRenderer>(batchShader, instanceShader);
    }

    void RenderSystem::createLineRenderers() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "line_batch",
                        "v_line_batch.glsl",
                        "f_line.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader();
        auto instanceShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "line_instance",
                        "v_line_instance.glsl",
                        "f_line.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        lineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE);
        stripLineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE_STRIP);
        loopLineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE_LOOP);
    }

    void RenderSystem::createQuadRenderer() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "quad_batch",
                        "v_quad_batch.glsl",
                        "f_quad.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader();
        auto instanceShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "quad_instance",
                        "v_quad_instance.glsl",
                        "f_quad.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        quadRenderer = createRef<MultiRenderer>(batchShader, instanceShader, TRIANGLE_STRIP);
    }

    void RenderSystem::createCircleRenderer() {
        auto circleArrayScript = ShaderScript();
        circleArrayScript.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto circles = registry.view<CircleComponent>();
            auto i = 0;
            for (auto [entity, circle] : circles.each()) {
                shader.setUniformArrayStructField(i, circle.name, circle.color);
                shader.setUniformArrayStructField(i, circle.name, circle.thickness);
                shader.setUniformArrayStructField(i++, circle.name, circle.fade);
            }
        };
        circleArrayScript.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto circle = entity.getPtr<CircleComponent>();
            if (circle) {
                shader.setUniformStructField(circle->name, circle->color);
                shader.setUniformStructField(circle->name, circle->thickness);
                shader.setUniformStructField(circle->name, circle->fade);
            }
        };

        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader({ circleArrayScript });
        auto batchShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "circle_batch",
                        "v_circle_batch.glsl",
                        "f_circle.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader({ circleArrayScript });
        auto instanceShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "circle_instance",
                        "v_circle_instance.glsl",
                        "f_circle.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        circleRenderer = createRef<MultiRenderer>(batchShader, instanceShader, TRIANGLE_STRIP);
    }

    void RenderSystem::createOutlineRenderer() {
        auto outlineScript = ShaderScript();

        outlineScript.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            auto outlines = registry.view<OutlineComponent>();
            auto i = 0;
            for (auto [entity, outline] : outlines.each()) {
                shader.setUniformArrayStructField(i, outline.name, outline.color);
                shader.setUniformArrayStructField(i++, outline.name, outline.thickness);
            }
        };

        outlineScript.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto outline = entity.getPtr<OutlineComponent>();
            if (outline) {
                shader.setUniformStructField(outline->name, outline->color);
                shader.setUniformStructField(outline->name, outline->thickness);
            }
        };

        auto vBatchShader = shader::BaseShader({ camera3dUboScript(), outlineScript });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "outline_batch",
                        "v_outline_batch.glsl",
                        "f_outline.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript(), outlineScript });
        auto fInstanceShader = shader::BaseShader();
        auto instanceShader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "outline_instance",
                        "v_outline_instance.glsl",
                        "f_outline.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        outlineSceneRenderer = createRef<MultiRenderer>(batchShader, instanceShader, TRIANGLE);
        outlineQuadRenderer = createRef<MultiRenderer>(batchShader, instanceShader, TRIANGLE_STRIP);
    }

    void RenderSystem::createSkyboxRenderer() {
        auto skyboxRotation = ShaderScript();

        skyboxRotation.updateRegistry = [](const BaseShader& shader, entt::registry& registry) {
            // do nothing
        };
        skyboxRotation.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto transform = entity.getPtr<Transform3dComponent>();
            if (transform) {
                transform->rotation.y += 0.0001f;
                updateModel3d(*transform);
            }
        };

        auto vShader = shader::BaseShader({ camera3dUboScript(), skyboxRotation });
        auto fShader = shader::BaseShader({ cubeMapTextureScript() });
        auto shader = createRef<shader::BaseShaderProgram>(
                shader::ShaderProps {
                        "cubemap",
                        "v_cubemap.glsl",
                        "f_cubemap.glsl",
                        ENGINE_SHADERS_PATH
                },
                vShader,
                fShader
        );

        skyboxRenderer = createRef<VRenderer>(shader);
    }
}
