//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/buffers/VertexBuffer.h"

namespace engine {

    class GLVertexBuffer : public VertexBuffer {

    public:
        GLVertexBuffer() : VertexBuffer() {
            create();
        }

        GLVertexBuffer(VertexFormat* vertexFormat) : VertexBuffer(vertexFormat) {
            create();
        }

        ~GLVertexBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

        void allocate() override;
        void setAttributesPointer() override;
        void enableAttributes() override;
        void disableAttributes() override;

        void load(const VertexData &vertexData) override;

        void recreate() override;

    private:
        void create();
        void destroy();

    };

}