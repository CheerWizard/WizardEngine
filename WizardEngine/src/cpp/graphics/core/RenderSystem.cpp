//
// Created by mecha on 05.09.2021.
//

#include <graphics/core/RenderSystem.h>
#include <graphics/camera/CameraShaderScript.h>
#include <graphics/material/MaterialShaderScript.h>
#include <graphics/light/LightShaderScript.h>

namespace engine {

    void RenderSystem::create() {
        createSceneRenderers();
        createLineRenderers();
    }

    void RenderSystem::onUpdate() {
        sceneFrame->bind();

        setClearColor({0.2, 0.2, 0.2, 1});
        setDepthTest(true);
        clearDepthBuffer();

//        setStencilTest(true);
//        setStencilTestActions({ KEEP, KEEP, REPLACE });
//        clearStencilBuffer();

//        setStencilTestOperator(ALWAYS, 1, false);
//        stencilMask(false);

        if (!activeScene->isEmpty()) {
            auto& registry = activeScene->getRegistry();
            sceneRenderer->renderVI<Vertex3d>(registry);
            lineRenderer->renderV<LineVertex>(registry);
            stripLineRenderer->renderV<LineVertex>(registry);
            loopLineRenderer->renderV<LineVertex>(registry);
        }

        sceneFrame->unbind();

//        setStencilTestOperator(NOT_EQUAL, 1, false);
//        stencilMask(true);

        setDepthTest(false);
        clearColorBuffer();
    }

    void RenderSystem::createSceneRenderers() {
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
}
