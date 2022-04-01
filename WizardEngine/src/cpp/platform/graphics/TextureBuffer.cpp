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

    GLenum toGLTextureParamName(const TextureParamName& textureParamName) {
        switch (textureParamName) {
            case TextureParamName::MAG_FILTER: return GL_TEXTURE_MAG_FILTER;
            case TextureParamName::MIN_FILTER: return GL_TEXTURE_MIN_FILTER;
            case TextureParamName::WRAP_R: return GL_TEXTURE_WRAP_R;
            case TextureParamName::WRAP_S: return GL_TEXTURE_WRAP_S;
            case TextureParamName::WRAP_T: return GL_TEXTURE_WRAP_T;
        }
    }

    GLint toGLTextureParamValue(const TextureParamValue& textureParamValue) {
        switch (textureParamValue) {
            case TextureParamValue::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case TextureParamValue::LINEAR: return GL_LINEAR;
            case TextureParamValue::REPEAT: return GL_REPEAT;
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

    void TextureBuffer::bind(const uint32_t& id) {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void TextureBuffer::unbind() const {
        glBindTexture(type, 0);
    }

    void TextureBuffer::activate(const uint32_t &slot) {
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

        setParams({
            { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_R, TextureParamValue::CLAMP_TO_EDGE },
        });
    }

    void TextureBuffer::load(const TextureData &textureData) {
        GLenum internalFormat = 0, dataFormat = 0;
        int channels = textureData.channels, width = textureData.width, height = textureData.height;

        switch (channels) {
            case CHANNEL_RED:
                internalFormat = GL_RED;
                dataFormat = GL_RED;
                break;
            case CHANNEL_RGB:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case CHANNEL_RGBA:
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
        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, textureData.data);
    }

    void TextureBuffer::load(const TextureFaceType &faceType, const TextureData &textureData) {
        GLint internalFormat = 0, dataFormat = 0;
        int channels = textureData.channels, width = textureData.width, height = textureData.height;

        switch (channels) {
            case CHANNEL_RED:
                internalFormat = GL_RED;
                dataFormat = GL_RED;
                break;
            case CHANNEL_RGB:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case CHANNEL_RGBA:
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

        glTexImage2D(toGLTextureFaceType(faceType), 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, textureData.data);
    }

    void TextureBuffer::disableByteAlignment() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void TextureBuffer::setParams(const std::vector<TextureParam>& params) const {
        for (auto& param : params) {
            glTextureParameteri(id, toGLTextureParamName(param.name), toGLTextureParamValue(param.value));
        }
    }
}