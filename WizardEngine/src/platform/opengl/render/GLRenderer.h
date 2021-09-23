//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/render/Renderer.h"

namespace engine {

    class GLRenderer : public Renderer {

    public:
        GLRenderer(ShaderCache* shaderCache,
                   GraphicsObjectCache* graphicsObjectCache,
                   const Ref<VertexArray> &vertexArray) :
        Renderer(shaderCache, graphicsObjectCache, vertexArray) {}

        ~GLRenderer() override {
            destroy();
        }

    protected:
        void drawIndices(const uint32_t &indexCount) override;

    };

}