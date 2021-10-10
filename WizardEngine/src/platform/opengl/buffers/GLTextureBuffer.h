//
// Created by mecha on 25.09.2021.
//

#pragma once

#include "../../../graphics/buffers/TextureBuffer.h"

namespace engine {

    class GLTextureBuffer : public TextureBuffer {

    public:
        GLTextureBuffer() : TextureBuffer() {
            create();
        }

        ~GLTextureBuffer() override {
            destroy();
        }

        void recreate() override;

    private:
        void create();
        void destroy();

        void bind() override;
        void unbind() override;

        void activate(const uint32_t &slot) override;

        bool load(const std::string &imageName) override;
        bool loadData(const void *data) override;

    };

}