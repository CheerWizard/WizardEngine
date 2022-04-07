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
#include <graphics/core/geometry/Point.h>

namespace engine::graphics {

    void RenderSystem::create() {
        createScreenRenderer();
        createSceneRenderer();
        createLineRenderers();
        createQuadRenderer();
        createCircleRenderer();
        createOutlineRenderer();
        createSkyboxRenderer();
        createTextRenderers();
        createPointRenderer();
        setClearColor({0, 0, 0, 1});
    }

    RenderSystem::~RenderSystem() {
        screenRenderer.release();
        sceneRenderer.release();
        lineRenderer.release();
        quadRenderer.release();
        circleRenderer.release();
        outlineSceneRenderer.release();
        outlineQuadRenderer.release();
        skyboxRenderer.release();
        text2dRenderer.release();
        text3dRenderer.release();
        pointRenderer.release();
    }

    void RenderSystem::onUpdate() {
        sceneFrame->bind();

        clearDepthBuffer();
        setDepthTest(true);

        // enables transparency
        setBlendMode(true);
        setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        setStencilTest(true);
        setStencilTestActions({ KEEP, KEEP, REPLACE });
        clearStencilBuffer();
        setStencilTestOperator(ALWAYS, 1, false);
        stencilMask(false);

        auto& registry = activeScene->getRegistry();
        // scene
        sceneRenderer.renderVI<Transform3dComponent, Vertex3d>(registry);
        // lines
        lineRenderer.renderV<Transform3dComponent, LineVertex>(registry);
        stripLineRenderer.renderV<Transform3dComponent, LineVertex>(registry);
        loopLineRenderer.renderV<Transform3dComponent, LineVertex>(registry);
        // quads
        quadRenderer.renderV<Transform3dComponent, QuadVertex>(registry);
        // circles
        circleRenderer.renderV<Transform3dComponent, CircleVertex>(registry);
        // text
        text2dRenderer.render<Text2d>(registry);
        text3dRenderer.render<Text3d>(registry);
        // points
        registry.view<Points>().each([=](auto entity, auto& points) {
            pointRenderer.render(points);
        });
        // outlining
        // stop write to stencil buffer
        setStencilTestOperator(NOT_EQUAL, 1, false);
        stencilMask(true);
        setDepthTest(false);
        outlineSceneRenderer.render<Transform3dComponent, OutlineVertex>(registry);
        // write to stencil buffer
        stencilMask(false);
        setStencilTestOperator(ALWAYS, 0, false);
        setDepthTest(true);
        // skybox
        setDepthTestOperator(LESS_EQUAL); // we need to pass depth test for some skybox pixels
        skyboxRenderer.render<Transform3dComponent, SkyboxVertex>(activeScene->getSkybox());
        setDepthTestOperator(LESS);
        // read/write from scene frame into screen frame
        FrameBuffer::readWriteFrameBuffers(*sceneFrame.get(), *screenFrame.get());
        // bind to window frame buffer and draw screen
        setDepthTest(false);
        FrameBuffer::bindDefault();
        screenRenderer.renderQuad(screenFrame->getColorAttachment(0).id);

        activeScene->updateComponents<Transform3dComponent>([](Transform3dComponent& transform) {
            transform.isUpdated = false;
        });
    }

    void RenderSystem::createSceneRenderer() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader({ materialScript(), phongLightScript(), materialMapScript(), pointLightArrayScript() });
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
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
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "instance",
                        "v_instance.glsl",
                        "f_instance.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );
        sceneRenderer = MultiRenderer(batchShader, instanceShader);
    }

    void RenderSystem::createLineRenderers() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
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
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "line_instance",
                        "v_line_instance.glsl",
                        "f_line.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        lineRenderer = MultiRenderer(batchShader, instanceShader, DrawType::LINE);
        stripLineRenderer = MultiRenderer(batchShader, instanceShader, DrawType::LINE_STRIP);
        loopLineRenderer = MultiRenderer(batchShader, instanceShader, DrawType::LINE_LOOP);
    }

    void RenderSystem::createQuadRenderer() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
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
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "quad_instance",
                        "v_quad_instance.glsl",
                        "f_quad.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        quadRenderer = MultiRenderer(batchShader, instanceShader, DrawType::QUAD);
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

        auto vBatchShader = shader::BaseShader();
        auto fBatchShader = shader::BaseShader({ circleArrayScript });
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "circle_batch",
                        "v_circle_batch.glsl",
                        "f_circle.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader();
        auto fInstanceShader = shader::BaseShader({ circleArrayScript });
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "circle_instance",
                        "v_circle_instance.glsl",
                        "f_circle.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        circleRenderer = MultiRenderer(batchShader, instanceShader, DrawType::QUAD);
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

        auto vBatchShader = shader::BaseShader({ outlineScript });
        auto fBatchShader = shader::BaseShader();
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "outline_batch",
                        "v_outline_batch.glsl",
                        "f_outline.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );
        auto vInstanceShader = shader::BaseShader({ outlineScript });
        auto fInstanceShader = shader::BaseShader();
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "outline_instance",
                        "v_outline_instance.glsl",
                        "f_outline.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        outlineSceneRenderer = MultiRenderer(batchShader, instanceShader, DrawType::TRIANGLE);
        outlineQuadRenderer = MultiRenderer(batchShader, instanceShader, DrawType::TRIANGLE_STRIP);
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
        auto shader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "cubemap",
                        "v_cubemap.glsl",
                        "f_cubemap.glsl",
                        ENGINE_SHADERS_PATH
                },
                vShader,
                fShader
        );

        skyboxRenderer = VRenderer(shader);
    }

    void RenderSystem::createTextRenderers() {
        auto fTextShader = shader::BaseShader();

        auto vText2dShader = shader::BaseShader({ textProjectionScript() });
        auto text2dShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "text2d",
                        "text/v_text_2d.glsl",
                        "text/f_text.glsl",
                        ENGINE_SHADERS_PATH
                },
                vText2dShader,
                fTextShader
        );
        text2dRenderer = TextRenderer(text2dShader);

        auto vText3dShader = shader::BaseShader();
        auto text3dShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "text3d",
                        "text/v_text_3d.glsl",
                        "text/f_text.glsl",
                        ENGINE_SHADERS_PATH
                },
                vText3dShader,
                fTextShader
        );
        text3dRenderer = TextRenderer(text3dShader);
    }

    void RenderSystem::createScreenRenderer() {
        auto vShader = shader::BaseShader();
        auto fShader = shader::BaseShader();
        auto shader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "screen",
                        "v_screen.glsl",
                        "f_screen.glsl",
                        ENGINE_SHADERS_PATH
                },
                vShader,
                fShader
        );
        screenRenderer = VRenderer(shader);

        // upload screen geometry to GPU memory and delete from heap memory
        auto screen = Screen();
        screenRenderer.uploadStatic(screen);
        delete screen.vertexData.vertices;
    }

    void RenderSystem::createPointRenderer() {
        auto shader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "point",
                        "geometry/v_point.glsl",
                        "geometry/f_point.glsl",
                        "geometry/g_point.glsl",
                        ENGINE_SHADERS_PATH
                }
        );

        pointRenderer = VRenderer(shader);
    }
}
