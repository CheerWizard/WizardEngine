//
// Created by mecha on 25.09.2021.
//

#pragma once

#include "../../core/File.h"
#include "Buffer.h"
#include "TextureData.h"

#include "string"

#define TEXTURE_PATH "assets/textures"

namespace engine {

    class TextureBuffer : public Buffer {

    public:
        TextureBuffer() : Buffer() {}

    public:
        ~TextureBuffer() override {
            delete textureData;
        }

    public:
        virtual void activate(const uint32_t &slot) = 0;
        virtual bool loadData(const void* data) = 0;

    public:
        inline void setTexturesPath(const std::string &texturesPath) {
            this->texturesPath = texturesPath;
        }

        inline const std::string& getTexturesPath() {
            return texturesPath;
        }

    public:
        virtual bool load(const std::string &imageName);
        void free();

    protected:
        TextureData* textureData = nullptr;

    private:
        std::string texturesPath = TEXTURE_PATH;

    };

}