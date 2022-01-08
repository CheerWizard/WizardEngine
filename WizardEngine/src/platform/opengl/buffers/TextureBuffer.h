//
// Created by mecha on 25.09.2021.
//

#pragma once

#include "../../../graphics/core/io/TextureFile.h"

#include "string"

namespace engine {

    class TextureBuffer final {

    public:
        TextureBuffer() {
            create();
        }
        ~TextureBuffer() = default;

    public:
        [[nodiscard]] inline const uint32_t& getId() const {
            return id;
        }

    public:
        // lifetime functions
        void create();
        void destroy();
        void recreate();
        // bind/unbind tbo
        void bind() const;
        void unbind();
        // activating tbo
        void activate(const uint32_t &slot) const;
        // streaming from file into tbo
        void loadFrom(const std::string_view &fileName);
        void loadFrom(const std::string_view &fileName, const std::string_view &texturesPath);
        // loading texture data into tbo
        void load(const TextureData &textureData);

    private:
        uint32_t id;
    };
}