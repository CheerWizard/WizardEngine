//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <platform/graphics/TextureBuffer.h>

#include "unordered_map"

#define GET_TEXTURE(path) engine::TextureSource::get()->getTextureBuffer(fileName)
#define GET_TEXTURE_ID(path) GET_TEXTURE(path).getId()
#define LOAD_TEXTURE(fileName, assetPath) engine::TextureSource::get()->loadTexture(fileName, assetPath)

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
        const TextureBuffer& getTextureBuffer(const std::string &fileName);
        const TextureBuffer& getTextureBuffer(const std::string &fileName, const std::string &texturesPath);

        /**
         * Checks if TextureBuffer already exists in cache by @param fileName.
         * 1. If it exists - load texture from this @param fileName.
         * 2. If does not exists - creates new TextureBuffer instance using GraphicsFactory,
         * save it by @param fileName into cache and proceed condition 1.
         * @return TextureBuffer ID from TextureBuffer that has loaded a texture from @param fileName
         */
        const uint32_t& loadTexture(const std::string &fileName);

        /**
         * Checks if TextureBuffer already exists in cache by @param fileName.
         * 1. If it exists - load texture from this @param fileName.
         * 2. If does not exists - creates new TextureBuffer instance using GraphicsFactory,
         * save it by @param fileName into cache and proceed condition 1.
         * @param texturesPath - path to actual textures folder!
         * @return TextureBuffer ID from TextureBuffer that has loaded a texture from @param fileName
         */
        const uint32_t& loadTexture(const std::string &fileName, const std::string &texturesPath);

        void remove(const std::string &fileName);
        void clear();

    public:
        const TextureBuffer& operator [](const std::string &name) {
            return getTextureBuffer(name);
        }

    private:
        bool exists(const std::string &name);

    private:
        Textures _textures;

    private:
        static Ref<TextureSource> instance;

    };

}