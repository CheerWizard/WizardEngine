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

        if (!activeScene->batchEmpty()) {
//            sceneBatchRenderer->renderVI<Vertex3d>(activeScene->getBatchRegistry());
            lineBatchRenderer->renderV<LineVertex>(activeScene->getBatchRegistry());
            stripLineBatchRenderer->renderV<LineVertex>(activeScene->getBatchRegistry());
            loopLineBatchRenderer->renderV<LineVertex>(activeScene->getBatchRegistry());
        }

        if (!activeScene->instanceEmpty()) {
//            sceneInstanceRenderer->renderVI<Vertex3d>(activeScene->getInstanceRegistry());
            lineInstanceRenderer->renderV<LineVertex>(activeScene->getInstanceRegistry());
            stripLineInstanceRenderer->renderV<LineVertex>(activeScene->getInstanceRegistry());
            loopLineInstanceRenderer->renderV<LineVertex>(activeScene->getInstanceRegistry());
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
        sceneBatchRenderer = createRef<BatchRenderer>(batchShader);
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
        sceneInstanceRenderer = createRef<InstanceRenderer>(instanceShader);
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
        lineBatchRenderer = createRef<BatchRenderer>(batchShader, LINE);
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
        lineInstanceRenderer = createRef<InstanceRenderer>(instanceShader, LINE);

        stripLineBatchRenderer = createRef<BatchRenderer>(batchShader, LINE_STRIP);
        stripLineInstanceRenderer = createRef<InstanceRenderer>(instanceShader, LINE_STRIP);

        loopLineBatchRenderer = createRef<BatchRenderer>(batchShader, LINE_LOOP);
        loopLineInstanceRenderer = createRef<InstanceRenderer>(instanceShader, LINE_LOOP);
    }

}
