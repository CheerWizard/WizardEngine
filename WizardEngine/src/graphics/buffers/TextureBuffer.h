//
// Created by mecha on 25.09.2021.
//

#pragma once

#include "../../core/File.h"
#include "Buffer.h"
#include "TextureData.h"

#include "string"

#define TEXTURE_PATHS "textures"

namespace engine {

    class TextureBuffer : public File, public Buffer {

    public:
        TextureBuffer() : File(), Buffer() {}

    public:
        ~TextureBuffer() override {
            delete textureData;
        }

    public:
        virtual void activate(const uint32_t &slot) = 0;
        virtual bool loadData(const void* data) = 0;

    public:
        virtual bool load(const std::string &filePath);
        void free();

    protected:
        const char *getExtensionName() const override;
        const char *getAssetPath() const override;

    protected:
        TextureData* textureData = nullptr;

    };

}