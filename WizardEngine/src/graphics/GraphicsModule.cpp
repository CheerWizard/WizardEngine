//
// Created by mecha on 01.11.2021.
//

#include "GraphicsModule.h"

#include "camera/CameraShaderController.h"
#include "transform/TransformShaderController.h"
#include "material/MaterialShaderController.h"
#include "light/LightShaderController.h"

namespace engine {

    Scope<MeshSource> GraphicsModule::newMeshSource() {
        return createScope<MeshSource>();
    }

    Ref<TextureSource> GraphicsModule::newTextureSource() {
        return createRef<TextureSource>(graphicsFactory);
    }

    Scope<ShaderSource> GraphicsModule::newShaderSource() {
        return createScope<ShaderSource>(graphicsFactory);
    }

    Scope<RenderSystem> GraphicsModule::newRenderSystem(
            const Ref<TextureSource> &textureSource,
            const Scope<ShaderSource> &shaderSource,
            const Ref<FrameController> &frameController
    ) {
        auto batchShader = shaderSource->create(ShaderProps {
            "batch",
            "v_batch",
            "f_batch",
            ENGINE_SHADERS_PATH
        });
        auto instanceShader = shaderSource->create(ShaderProps {
            "instance",
            "v_instance",
            "f_instance",
            ENGINE_SHADERS_PATH
        });

        auto batchRenderer = newTextureRenderer(batchShader, textureSource);
        auto instanceRenderer = newTextureRenderer(instanceShader, textureSource);

        return createScope<RenderSystem>(frameController, batchRenderer, instanceRenderer);
    }

    Ref<FrameController> GraphicsModule::newFrameController() {
        return createRef<FrameController>(graphicsFactory->newFrameBuffer());
    }

    Ref<Renderer> GraphicsModule::newTextureRenderer(const Ref<Shader> &shader, const Ref<TextureSource> &textureSource) {
        auto renderer = createRef<Renderer>(shader, graphicsFactory);
        provideShaderControllers(renderer, textureSource);
        return renderer;
    }

    void GraphicsModule::provideShaderControllers(Ref<Renderer> &renderer, const Ref<TextureSource> &textureSource) {
        renderer->add(createRef<CameraShaderController>());
        renderer->add(createRef<TransformShaderController>());
        renderer->add(createRef<MaterialShaderController>(textureSource));
        renderer->add(createRef<LightShaderController>());
    }

    Ref<RenderSettings> GraphicsModule::newRenderSettings() {
        return graphicsFactory->newRenderSettings();
    }
}