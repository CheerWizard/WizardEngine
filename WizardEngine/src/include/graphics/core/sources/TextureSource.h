//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <platform/graphics/TextureBuffer.h>

#include "unordered_map"

#define GET_TEXTURE(texture, assetPath) engine::TextureSource::get()->getTextureBuffer(texture, assetPath)
#define GET_TEXTURE_ID(texture, assetPath) GET_TEXTURE(texture, assetPath).getId()
#define LOAD_TEXTURE(texture, assetPath) engine::TextureSource::get()->loadTexture(texture, assetPath)
#define LOAD_TEXTURE_PARAMS(type, fileName, assetPath) engine::TextureSource::get()->loadTexture(type, fileName, assetPath)
#define ACTIVATE_TEXTURE(texture) engine::TextureSource::get()->activate(texture)

namespace engine {

    typedef std::unordered_map<std::string, TextureBuffer> Textures;
    typedef Textures::iterator TextureIterator;

    class TextureSource {

    public:
        TextureSource() = default;
        ~TextureSource() {
            clear();
        }

    public:
        static const Ref<TextureSource>& get() {
            return instance;
        }

    public:
        inline TextureIterator begin() {
            return _textures.begin();
        }

        inline TextureIterator end() {
            return _textures.end();
        }

    public:
        const TextureBuffer& getTextureBuffer(const TextureComponent& textureComponent);
        const TextureBuffer& getTextureBuffer(
                const TextureComponent& textureComponent,
                const std::string& texturesPath
        );
        const TextureBuffer& getTextureBuffer(
                const TextureType& type,
                const std::string& fileName,
                const std::string& texturesPath
        );
        const TextureBuffer& getTextureBuffer(
                const CubeMapTextureComponent& cubeMapTextureComponent,
                const std::string& texturesPath
        );

        /**
         * Checks if TextureBuffer already exists in cache by @param fileName.
         * 1. If it exists - load texture from this @param fileName.
         * 2. If does not exists - creates new TextureBuffer instance using GraphicsFactory,
         * save it by @param fileName into cache and proceed condition 1.
         * @return TextureBuffer ID from TextureBuffer that has loaded a texture from @param fileName
         */
        uint32_t loadTexture(const TextureComponent& textureComponent);

        /**
         * Checks if TextureBuffer already exists in cache by @param fileName.
         * 1. If it exists - load texture from this @param fileName.
         * 2. If does not exists - creates new TextureBuffer instance using GraphicsFactory,
         * save it by @param fileName into cache and proceed condition 1.
         * @param texturesPath - path to actual textures folder!
         * @return TextureBuffer ID from TextureBuffer that has loaded a texture from @param fileName
         */
        uint32_t loadTexture(
                const TextureComponent& textureComponent,
                const std::string &texturesPath
        );

        uint32_t loadTexture(
                const TextureType& type,
                const std::string& fileName,
                const std::string& texturesPath
        );

        uint32_t loadTexture(
                const CubeMapTextureComponent& cubeMapTextureComponent,
                const std::string& texturesPath
        );

        void activate(const TextureComponent& textureComponent);
        void activate(const CubeMapTextureComponent& cubeMapTextureComponent);

        void remove(const std::string &fileName);
        void clear();

    private:
        bool exists(const std::string &name);

    private:
        Textures _textures;

    private:
        static Ref<TextureSource> instance;

    };

}