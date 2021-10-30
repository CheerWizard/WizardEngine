//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "Renderer.h"

#include "../GraphicsFactory.h"

#include "../../ecs/System.h"

#include "../geometry/MeshComponent.h"
#include "../../transform/TransformComponents.h"
#include "material/MaterialComponents.h"

#include "../frame/FrameController.h"

#include "../sources/ShaderSource.h"

#include "string"

#define INSTANCE_COUNT_LIMIT 128

namespace engine {

    class RenderSystem : public System {

    public:
        RenderSystem(
                const Ref<GraphicsFactory> &graphicsFactory,
                const Ref<FrameController> &frameController,
                const Ref<ShaderSource> &shaderSource,
                const Ref<TextureSource> &textureSource
        ) : frameController(frameController) {
            create(graphicsFactory, shaderSource, textureSource);
        }

        ~RenderSystem() override {
            destroy();
        }

    public:
        void onUpdate() override;

    private:
        void create(
                const Ref<GraphicsFactory>& graphicsFactory,
                const Ref<ShaderSource> &shaderSource,
                const Ref<TextureSource> &textureSource
        );
        void destroy();
        void renderBatching(const std::vector<Entity> &entities);
        void renderInstancing(const Family& family);

    protected:
        Ref<FrameController> frameController;
        Ref<Renderer> batchRenderer;
        Ref<Renderer> instanceRenderer;

    };

}