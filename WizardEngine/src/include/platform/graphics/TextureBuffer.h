//
// Created by mecha on 25.09.2021.
//

#pragma once

#include <graphics/core/io/TextureFile.h>
#include <graphics/core/texture/Texture.h>

#include <string>

namespace engine::graphics {

    class ENGINE_API TextureBuffer final {

    public:
        TextureBuffer() = default;
        TextureBuffer(u32 type) {
            create(type);
        }
        ~TextureBuffer() = default;

    public:
        [[nodiscard]] inline u32 getId() const {
            return id;
        }

        [[nodiscard]] inline u32 getType() const {
            return type;
        }

    public:
        void create(u32 textureType);
        void bind() const;
        void unbind() const;
        void setParams(const vector<TextureParam>& params) const;

    public:
        static u32 createFrom(u32 type);
        static u32 recreate(u32 id, u32 typeId);
        static void destroy(u32 id);

        static void bind(u32 id, u32 type);
        static void unbind(u32 type);

        static void activate(u32 slot);
        static void setParams(u32 id, const vector<TextureParam>& params);
        static void setDefaultParamsCubeMap(u32 id);
        static void disableByteAlignment();
        // read from file and load into texture buffer
        static u32 upload(const io::TextureData& textureData);
        static u32 upload(const vector<std::pair<u32, io::TextureData>>& textures);
        static u32 uploadArray(const io::TextureArrayData& textureArrayData);
        static u32 generateCubeMap(int width, int height,
                                   int internalFormat, u32 dataFormat, u32 pixelsType);
        // load texture data into texture buffer
        static void load(const io::TextureData &textureData);
        static void loadFace(u32 type, const io::TextureData& textureData);
        static void loadArray(const io::TextureArrayData& textureArrayData);

    private:
        u32 id = 0;
        u32 type = 0;
    };
}