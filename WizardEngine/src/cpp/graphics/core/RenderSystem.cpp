//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>
#include <graphics/camera/CameraShaderScript.h>
#include <graphics/light/LightShaderScript.h>
#include <graphics/core/geometry/Line.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/core/geometry/Circle.h>
#include <graphics/outline/Outline.h>
#include <graphics/core/geometry/Point.h>

namespace engine::graphics {
    // frames
    Ref<Scene> RenderSystem::activeScene;
    Ref<FrameBuffer> RenderSystem::sceneFrame;
    Ref<FrameBuffer> RenderSystem::screenFrame;
    // screen
    VRenderer<ScreenVertex> RenderSystem::screenRenderer;
    // skybox
    VRenderer<SkyboxVertex> RenderSystem::skyboxRenderer;
    // scene
    vector<Ref<Renderer>> RenderSystem::sceneRenderers;
    // outlining
    vector<Ref<Renderer>> RenderSystem::outlineRenderers;
    // points
//    VRenderer<PointVertex> RenderSystem::pointRenderer;
    // text
    vector<Ref<Renderer>> RenderSystem::textRenderers;
    // callback
    RenderSystemCallback* RenderSystem::callback;

    void RenderSystem::initDefault() {
        createScreenRenderer();
        createSkyboxRenderer();
        createSceneRenderer();
//        createLineRenderer();
//        createQuadRenderer();
//        createCircleRenderer();
//        createOutlineRenderer();
//        createTextRenderers();
//        createPointRenderer();
        setClearColor({0, 0, 0, 1});
    }

    void RenderSystem::onDestroy() {
        screenRenderer.release();
        skyboxRenderer.release();
//        pointRenderer.release();
        for (const auto& sceneRenderer : sceneRenderers) {
            sceneRenderer->release();
        }
        for (const auto& outlineRenderer : outlineRenderers) {
            outlineRenderer->release();
        }
        for (const auto& textRenderer : textRenderers) {
            textRenderer->release();
        }
    }

    void RenderSystem::onUpdate() {
        sceneFrame->bind();

        clearDepthBuffer();
        setDepthTest(true);

        // notify that scene frame begin drawing
        if (callback != nullptr) {
            callback->onFrameBegin(sceneFrame);
        }

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
        for (const auto& renderer : sceneRenderers) {
            renderer->render(registry);
        }
        // text
        for (const auto& textRenderer : textRenderers) {
            textRenderer->render(registry);
        }
        // points
//        registry.each<Points>([](Points* points) {
//            pointRenderer.render(*points);
//        });
        // outlining
        // stop write to stencil buffer
        setStencilTestOperator(NOT_EQUAL, 1, false);
        stencilMask(true);
        setDepthTest(false);
        for (const auto& outlineRenderer : outlineRenderers) {
            outlineRenderer->render(registry);
        }
        // write to stencil buffer
        stencilMask(false);
        setStencilTestOperator(ALWAYS, 0, false);
        setDepthTest(true);
        // skybox
        setDepthTestOperator(LESS_EQUAL); // we need to pass depth test for some skybox pixels
        skyboxRenderer.renderStatic(activeScene->getSkybox());
        setDepthTestOperator(LESS);
        // notify that scene frame end drawing
        if (callback != nullptr) {
            callback->onFrameEnd(sceneFrame);
        }
        // read/write from scene frame into screen frame
        FrameBuffer::readWriteFrameBuffers(*sceneFrame.get(), *screenFrame.get());
        // bind to window frame buffer and draw screen
        setDepthTest(false);

        FrameBuffer::bindDefault();
        screenRenderer.renderQuad(screenFrame->getColorAttachment(0).id);
    }

    void RenderSystem::createSceneRenderer() {
        auto vBatchShader = shader::BaseShader({ camera3dUboScript() });
        auto fBatchShader = shader::BaseShader({ phongLightScript() });
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "batch",
                        "v_batch.glsl",
                        "scene_phong.glsl",
                        ENGINE_SHADERS_PATH
                },
                vBatchShader,
                fBatchShader
        );

        auto vInstanceShader = shader::BaseShader({ camera3dUboScript() });
        auto fInstanceShader = shader::BaseShader({ phongLightScript() });
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "instance",
                        "v_instance.glsl",
                        "scene_phong.glsl",
                        ENGINE_SHADERS_PATH
                },
                vInstanceShader,
                fInstanceShader
        );

        Ref<Renderer> batchRenderer = createRef<BatchRenderer<Vertex3d>>(batchShader);
        Ref<Renderer> instanceRenderer = createRef<InstanceRenderer<Vertex3d>>(instanceShader);
        auto entityHandler = [](ecs::Registry& registry,
        ecs::entity_id entityId, u32 index, BaseShaderProgram& shader) {
            int entityIdInt = (int) entityId;
//            ENGINE_INFO("entityHandler: entity_id: {0}, index: {1}", entityIdInt, index);
            // update polygon mode
            auto* pmc = registry.getComponent<PolygonModeComponent>(entityId);
            PolygonModes::setPolygonMode(pmc ? *pmc : PolygonModeComponent());
            // update culling mode
            auto* culling = registry.getComponent<CullingComponent>(entityId);
            Culling::setCulling(culling ? *culling : CullingComponent());
            // pass entity id to shader
            IntUniform entityIdUniform = { "entityId", entityIdInt };
            shader.getFShader().setUniformArrayElement(index, entityIdUniform);
            // update single material
            auto material = registry.getComponent<Material>(entityId);
            if (material) {
                MaterialShader(shader.getFShader()).setMaterial(index, material);
            }
            // update material list
            auto materialList = registry.getComponent<MaterialList>(entityId);
            if (materialList) {
                for (u32 i = 0; i < materialList->materials.size(); i++) {
                    auto& m = materialList->materials[i];
                    MaterialShader(shader.getFShader()).setMaterial(index + i, &m);
                }
            }
        };

        batchRenderer->addEntityHandler(entityHandler);
        instanceRenderer->addEntityHandler(entityHandler);

        sceneRenderers.emplace_back(batchRenderer);
        sceneRenderers.emplace_back(instanceRenderer);
    }

    void RenderSystem::createLineRenderer() {
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

        sceneRenderers.emplace_back(createRef<BatchRenderer<LineVertex>>(
                batchShader,
                DrawType::LINE));
        sceneRenderers.emplace_back(createRef<InstanceRenderer<LineVertex>>(
                instanceShader,
                DrawType::LINE));

        sceneRenderers.emplace_back(createRef<BatchRenderer<LineVertex>>(
                batchShader,
                DrawType::LINE_STRIP));
        sceneRenderers.emplace_back(createRef<InstanceRenderer<LineVertex>>(
                instanceShader,
                DrawType::LINE_STRIP));

        sceneRenderers.emplace_back(createRef<BatchRenderer<LineVertex>>(
                batchShader,
                DrawType::LINE_LOOP));
        sceneRenderers.emplace_back(createRef<InstanceRenderer<LineVertex>>(
                instanceShader,
                DrawType::LINE_LOOP));
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

        sceneRenderers.emplace_back(createRef<BatchRenderer<QuadVertex>>(batchShader, DrawType::QUAD));
        sceneRenderers.emplace_back(createRef<InstanceRenderer<QuadVertex>>(instanceShader, DrawType::QUAD));
    }

    void RenderSystem::createCircleRenderer() {
        auto circleArrayScript = ShaderScript();

        circleArrayScript.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            auto i = 0;
            registry.each<CircleComponent>([&shader, &i](CircleComponent* circle) {
                shader.setUniformArrayStructField(i, circle->name, circle->color);
                shader.setUniformArrayStructField(i, circle->name, circle->thickness);
                shader.setUniformArrayStructField(i++, circle->name, circle->fade);
            });
        };

        circleArrayScript.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto circle = entity.get<CircleComponent>();
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

        sceneRenderers.emplace_back(createRef<BatchRenderer<CircleVertex>>(
                batchShader,
                DrawType::QUAD));
        sceneRenderers.emplace_back(createRef<InstanceRenderer<CircleVertex>>(
                instanceShader,
                DrawType::QUAD));
    }

    void RenderSystem::createOutlineRenderer() {
        auto outlineScript = ShaderScript();

        outlineScript.updateRegistry = [](const BaseShader& shader, ecs::Registry& registry) {
            auto i = 0;
            registry.each<OutlineComponent>([&shader, &i](OutlineComponent* outlineComponent) {
                auto& outline = *outlineComponent;
                shader.setUniformArrayStructField(i, outline.name, outline.color);
                shader.setUniformArrayStructField(i++, outline.name, outline.thickness);
            });
        };

        outlineScript.updateEntity = [](const BaseShader& shader, const ecs::Entity& entity) {
            auto outline = entity.get<OutlineComponent>();
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

        outlineRenderers.emplace_back(createRef<BatchRenderer<OutlineVertex>>(batchShader));
        outlineRenderers.emplace_back(createRef<InstanceRenderer<OutlineVertex>>(instanceShader));
        outlineRenderers.emplace_back(createRef<BatchRenderer<OutlineVertex>>(batchShader,
                DrawType::TRIANGLE_STRIP));
        outlineRenderers.emplace_back(createRef<InstanceRenderer<OutlineVertex>>(instanceShader,
                DrawType::TRIANGLE_STRIP));
    }

    void RenderSystem::createSkyboxRenderer() {
        auto vShader = shader::BaseShader({ camera3dUboScript() });
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

        skyboxRenderer = VRenderer<SkyboxVertex>(shader);
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
        textRenderers.emplace_back(createRef<TextRenderer<Text2d>>(text2dShader));

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
        textRenderers.emplace_back(createRef<TextRenderer<Text3d>>(text3dShader));
    }

    void RenderSystem::createScreenRenderer() {
        auto screen = Screen();
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
        // upload screen geometry to GPU memory and delete from heap memory
        screenRenderer = VRenderer<ScreenVertex>(shader);
        screenRenderer.uploadStatic(screen);
        delete screen.vertexData.values;
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

//        pointRenderer = VRenderer<PointVertex>(shader);
    }

    void RenderSystem::onPrepare() {
        uploadSkybox();
    }

    void RenderSystem::uploadSkybox() {
        auto skybox = activeScene->getSkybox().get<VertexDataComponent<SkyboxVertex>>();
        if (skybox->vertexData.values) {
            skyboxRenderer.uploadStatic(*skybox);
            delete skybox->vertexData.values;
        } else {
            ENGINE_WARN("RenderSystem: Skybox of '{0}' is already uploaded!", activeScene->getName());
        }
    }

    void RenderSystem::setRenderSystemCallback(RenderSystemCallback* renderSystemCallback) {
        callback = renderSystemCallback;
    }

    void RenderSystem::removeRenderSystemCallback() {
        callback = nullptr;
    }
}
