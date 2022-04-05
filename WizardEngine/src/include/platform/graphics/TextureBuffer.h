//
// Created by mecha on 25.09.2021.
//

#pragma once

#include <graphics/core/io/TextureFile.h>
#include <graphics/core/texture/Texture.h>

#include <string>

namespace engine::graphics {

    class TextureBuffer final {

    public:
        TextureBuffer() = default;
        TextureBuffer(const TextureType& type) {
            create(type);
        }
        ~TextureBuffer() = default;

    public:
        [[nodiscard]] inline uint32_t getId() const {
            return id;
        }

    public:
        static void disableByteAlignment();

    public:
        // lifetime functions
        void create(const TextureType& textureType);
        void destroy();
        void recreate();
        // bind/unbind tbo
        void bind() const;
        void unbind() const;
        // read from file and load to tbo
        void loadFrom(const std::string_view &fileName);
        void loadFrom(const std::string_view &fileName, const std::string_view &texturesPath);
        void loadTexture2d(const std::string_view& filename, const std::string_view& texturesPath);
        void loadCubeMap(const std::vector<TextureFace>& faces, const std::string_view& texturesPath);
        // loading texture data into tbo
        void load(const io::TextureData &textureData);
        void load(const TextureFaceType& faceType, const io::TextureData& textureData);
        void setParams(const std::vector<TextureParam>& params) const;

    public:
        static void bind(const uint32_t& id);
        static void activate(const uint32_t& slot);

    private:
        u32 id;
        u32 type;
    };
}