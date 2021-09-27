//
// Created by mecha on 25.09.2021.
//

#include "GLTextureBuffer.h"

#include "glad/glad.h"

#include "../../../core/Assert.h"

namespace engine {

    void GLTextureBuffer::create() {
        ENGINE_INFO("TextureBuffer : create()");
        glCreateTextures(GL_TEXTURE_2D, 1, &id);
    }

    void GLTextureBuffer::destroy() {
        ENGINE_INFO("TextureBuffer : destroy()");
        glDeleteTextures(1, &id);
    }

    void GLTextureBuffer::bind() {
        ENGINE_INFO("TextureBuffer : bind()");
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void GLTextureBuffer::unbind() {
        ENGINE_INFO("TextureBuffer : unbind()");
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTextureBuffer::activate(const uint32_t &slot) {
        ENGINE_INFO("TextureBuffer : activate()");
        glBindTextureUnit(GL_TEXTURE0 + slot, id);
    }

    bool GLTextureBuffer::load(const std::string &filePath) {
        auto isLoaded = TextureBuffer::load(filePath);
        if (!isLoaded) return false;
        return loadData(textureData->data);
    }

    bool GLTextureBuffer::loadData(const void* data) {
        GLenum internalFormat = 0, dataFormat = 0;
        int channels = textureData->channels, width = textureData->width, height = textureData->height;

        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        ENGINE_ASSERT(internalFormat & dataFormat, "TextureBuffer format is not supported!")

        glTextureStorage2D(id, 1, internalFormat, width, height);

        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

        free();

        return true;
    }

}