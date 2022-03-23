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

namespace engine {

    void RenderSystem::create() {
        createSceneRenderer();
        createLineRenderers();
        createQuadRenderer();
        createCircleRenderer();
    }

    void RenderSystem::onUpdate() {
        if (activeScene->isEmpty()) return;

        sceneFrame->bind();

        // enables transparency
        setBlendMode(true);
        setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);

        setClearColor({0.2, 0.2, 0.2, 1});
        setDepthTest(true);
        clearDepthBuffer();

//        setStencilTest(true);
//        setStencilTestActions({ KEEP, KEEP, REPLACE });
//        clearStencilBuffer();

//        setStencilTestOperator(ALWAYS, 1, false);
//        stencilMask(false);

        auto& registry = activeScene->getRegistry();
        // scene
        sceneRenderer->renderVI<Vertex3d>(registry);
        // lines
        lineRenderer->renderV<LineVertex>(registry);
        stripLineRenderer->renderV<LineVertex>(registry);
        loopLineRenderer->renderV<LineVertex>(registry);
        // quads
        quadRenderer->renderV<QuadVertex>(registry);
        // circles
        circleRenderer->renderV<CircleVertex>(registry);
        // outline everything
        outlineSceneRenderer->render<OutlineVertex>(registry);
        outlineLineRenderer->render<OutlineVertex>(registry);
        outlineStripLineRenderer->render<OutlineVertex>(registry);
        outlineLoopLineRenderer->render<OutlineVertex>(registry);
        outlineQuadRenderer->render<OutlineVertex>(registry);

        sceneFrame->unbind();

//        setStencilTestOperator(NOT_EQUAL, 1, false);
//        stencilMask(true);

        setDepthTest(false);
        clearColorBuffer();
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
                shader.setUniformArrayStructField(i, circle.name, circle.thickness);
                shader.setUniformArrayStructField(i, circle.name, circle.fade);
            }
        };
        circleArrayScript.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto circle = entity.getPtr<CircleComponent>();
            if (circle) {
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
                shader.setUniformArrayElement(i, outline.color);
            }
        };
        outlineScript.updateEntity = [](const BaseShader& shader, const Entity& entity) {
            auto outline = entity.getPtr<OutlineComponent>();
            if (outline) {
                shader.setUniform(outline->color);
            }
        };

        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader({ outlineScript });
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
        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader({ outlineScript });
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
        outlineLineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE);
        outlineStripLineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE_STRIP);
        outlineLoopLineRenderer = createRef<MultiRenderer>(batchShader, instanceShader, LINE_LOOP);
        outlineQuadRenderer = createRef<MultiRenderer>(batchShader, instanceShader, TRIANGLE_STRIP);
    }
}
