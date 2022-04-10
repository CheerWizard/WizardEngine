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
            init(type);
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
        void init(const TextureType& textureType);
        void bind() const;
        void unbind() const;
        void setParams(const vector<TextureParam>& params) const;

    public:
        static u32 create(const TextureType& textureType);
        static u32 recreate(const u32& id, const u32& typeId);
        static void destroy(const u32& id);

        static void bind(const u32& id, const u32& typeId);
        static void unbind(const TextureType& type);

        static void activate(const u32& slot);
        static void setParams(const u32& id, const vector<TextureParam>& params);
        static void setDefaultParamsCubeMap(const u32& id);
        static void disableByteAlignment();
        // read from file and load into texture buffer
        static u32 load(const char* filePath);
        static u32 load(const vector<TextureFace>& faces);
        // load texture data into texture buffer
        static void load(const u32& id, const io::TextureData &textureData);
        static void load(const u32& id, const TextureFaceType& faceType, const io::TextureData& textureData);

        static u32 getTypeId(const TextureType& textureType);

    private:
        u32 id;
        u32 type;
    };
}