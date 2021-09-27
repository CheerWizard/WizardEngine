//
// Created by mecha on 27.09.2021.
//

#pragma once

#include "../../../graphics/buffers/FrameBuffer.h"

namespace engine {

    class GLFrameBuffer : FrameBuffer {

    public:
        GLFrameBuffer() {
            create();
        }

        ~GLFrameBuffer() override {
            destroy();
        }

    public:
        void bind() override;
        void unbind() override;

    private:
        void create();
        void destroy();

    };

}