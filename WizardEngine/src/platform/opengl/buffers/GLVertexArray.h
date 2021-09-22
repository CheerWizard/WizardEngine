//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "../../../graphics/buffers/VertexArray.h"

namespace engine {

    class GLVertexArray : public VertexArray {

    public:
        GLVertexArray(VertexBufferCache* vertexBufferCache,
                      const Ref<IndexBuffer>& indexBuffer) :
                      VertexArray(vertexBufferCache, indexBuffer) {
            create();
        }

        ~GLVertexArray() {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

    protected:
        void destroy() override;

    private:
        void create();

    };

}