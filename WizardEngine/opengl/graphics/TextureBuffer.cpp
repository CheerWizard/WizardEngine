//
// Created by mecha on 25.09.2021.
//

#include <platform/graphics/TextureBuffer.h>
#include "glad/glad.h"
#include <array>

namespace engine {

    void toGLTextureType(const TextureType& textureType, GLenum& glTextureType) {
        switch (textureType) {
            case TextureType::TEXTURE_2D:
                glTextureType = GL_TEXTURE_2D;
                break;
            case TextureType::CUBE_MAP:
                glTextureType = GL_TEXTURE_CUBE_MAP;
                break;
            default:
                glTextureType = GL_TEXTURE_2D;
                break;
        }
    }

    GLenum toGLTextureFaceType(const TextureFaceType& faceType) {
        switch (faceType) {
            case TextureFaceType::RIGHT: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case TextureFaceType::LEFT: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case TextureFaceType::TOP: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case TextureFaceType::BOTTOM: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case TextureFaceType::BACK: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            case TextureFaceType::FRONT: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        }
    }

    void TextureBuffer::create(const TextureType& textureType) {
        toGLTextureType(textureType, type);
        glCreateTextures(type, 1, &id);
    }

    void TextureBuffer::destroy() {
        glDeleteTextures(1, &id);
    }

    void TextureBuffer::recreate() {
        destroy();
        glCreateTextures(type, 1, &id);
    }

    void TextureBuffer::bind() const {
        glBindTexture(type, id);
    }

    void TextureBuffer::unbind() const {
        glBindTexture(type, 0);
    }

    void TextureBuffer::activate(const uint32_t &slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void TextureBuffer::loadFrom(const std::string_view &fileName) {
        loadFrom(fileName, RUNTIME_TEXTURES_PATH);
    }

    void TextureBuffer::loadFrom(const std::string_view &fileName, const std::string_view &texturesPath) {
        switch (type) {
            case GL_TEXTURE_2D:
                loadTexture2d(fileName, texturesPath);
                break;
        }
    }

    void TextureBuffer::loadTexture2d(const std::string_view& filename, const std::string_view &texturesPath) {
        auto textureData = TextureFile::read(filename, texturesPath);

        if (textureData.data == nullptr) {
            ENGINE_WARN("Can't stream texture {0} from NULL data!", filename);
        } else {
            load(textureData);
        }

        TextureFile::free(textureData.data);
    }

    void TextureBuffer::loadCubeMap(
            const std::vector<TextureFace>& faces,
            const std::string_view &texturesPath
    ) {
        for (auto& face : faces) {
            auto textureData = TextureFile::read(face.fileName, texturesPath);

            if (textureData.data == nullptr) {
                ENGINE_WARN("Can't stream texture {0} from NULL data!", face.fileName);
            } else {
                load(face.type, textureData);
            }

            TextureFile::free(textureData.data);
        }

        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void TextureBuffer::load(const TextureData &textureData) {
        GLenum internalFormat = 0, dataFormat = 0;
        int channels = textureData.channels, width = textureData.width, height = textureData.height;

        switch (channels) {
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            default: break;
        }

        bool formatSupported = internalFormat & dataFormat;
        if (!formatSupported) {
            ENGINE_WARN(
                    "TextureBuffer {0} does not support format! internalFormat: {1}, dataFormat: {2}",
                    id,
                    internalFormat,
                    dataFormat
            );
        }

        glTextureStorage2D(id, 1, internalFormat, width, height);
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, textureData.data);
    }

    void TextureBuffer::load(const TextureFaceType &faceType, const TextureData &textureData) {
        GLint internalFormat = 0, dataFormat = 0;
        int channels = textureData.channels, width = textureData.width, height = textureData.height;

        switch (channels) {
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            default: break;
        }

        bool formatSupported = internalFormat & dataFormat;
        if (!formatSupported) {
            ENGINE_WARN(
                    "TextureBuffer {0} does not support format! internalFormat: {1}, dataFormat: {2}",
                    id,
                    internalFormat,
                    dataFormat
            );
        }

        glTexImage2D(
                toGLTextureFaceType(faceType),
                0,
                internalFormat,
                width,
                height,
                0,
                dataFormat,
                GL_UNSIGNED_BYTE,
                textureData.data
        );
    }

}