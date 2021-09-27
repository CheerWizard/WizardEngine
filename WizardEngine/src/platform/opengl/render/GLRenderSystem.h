//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/render/RenderSystem.h"

namespace engine {

    class GLRenderSystem : public RenderSystem {

    public:
        GLRenderSystem(ShaderCache* shaderCache, const Ref<VertexArray> &vertexArray) :
                RenderSystem(shaderCache, vertexArray) {}

        ~GLRenderSystem() override {
            destroy();
        }

    protected:
        void drawIndices(const uint32_t &indexCount) override;

    };

}