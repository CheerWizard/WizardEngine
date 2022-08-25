//
// Created by mecha on 25.09.2021.
//

#include <platform/graphics/TextureBuffer.h>
#include "glad/glad.h"

namespace engine::graphics {

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

    u32 TextureBuffer::createFrom(const TextureType& textureType) {
        GLenum type;
        GLuint id;
        toGLTextureType(textureType, type);
        glCreateTextures(type, 1, &id);
        return id;
    }

    void TextureBuffer::destroy(const u32& id) {
        glDeleteTextures(1, &id);
    }

    u32 TextureBuffer::recreate(const u32& id, const u32& typeId) {
        destroy(id);
        u32 newId;
        glCreateTextures(typeId, 1, &newId);
        return newId;
    }

    void TextureBuffer::bind() const {
        glBindTexture(type, id);
    }

    void TextureBuffer::unbind() const {
        glBindTexture(type, 0);
    }

    void TextureBuffer::setParams(const std::vector<TextureParam>& params) const {
        for (auto& param : params) {
            glTextureParameteri(id, toGLTextureParamName(param.name), toGLTextureParamValue(param.value));
        }
    }

    void TextureBuffer::bind(const u32& id, const u32& typeId) {
        glBindTexture(typeId, id);
    }

    void TextureBuffer::unbind(const TextureType& type) {
        GLenum textureType;
        toGLTextureType(type, textureType);
        glBindTexture(textureType, 0);
    }

    void TextureBuffer::activate(const u32 &slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    u32 TextureBuffer::load(const char* filePath) {
        auto textureData = io::TextureFile::read(filePath);

        TextureBuffer textureBuffer;
        if (textureData.data == nullptr) {
            ENGINE_WARN("Can't stream texture {0} from NULL data!", filePath);
        } else {
            textureBuffer.create(TextureType::TEXTURE_2D);
            textureBuffer.bind();
            load(textureBuffer.id, textureData);
            textureBuffer.unbind();
        }
        io::TextureFile::free(textureData.data);

        return textureBuffer.id;
    }

    u32 TextureBuffer::load(const std::vector<TextureFace>& faces) {
        TextureBuffer textureBuffer(TextureType::CUBE_MAP);
        textureBuffer.bind();

        for (auto& face : faces) {
            auto textureData = io::TextureFile::read(face.filePath);

            if (textureData.data == nullptr) {
                ENGINE_WARN("Can't stream texture {0} from NULL data!", face.filePath);
            } else {
                load(textureBuffer.id, face.type, textureData);
            }

            io::TextureFile::free(textureData.data);
        }

        textureBuffer.unbind();
        return textureBuffer.id;
    }

    void TextureBuffer::load(const u32& id, const io::TextureData &textureData) {
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

    void TextureBuffer::load(const u32& id, const TextureFaceType &faceType, const io::TextureData &textureData) {
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

    void TextureBuffer::setParams(const u32& id, const std::vector<TextureParam>& params) {
        for (auto& param : params) {
            glTextureParameteri(id, toGLTextureParamName(param.name), toGLTextureParamValue(param.value));
        }
    }

    void TextureBuffer::setDefaultParamsCubeMap(const u32& id) {
        setParams(id, {
            { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_R, TextureParamValue::CLAMP_TO_EDGE },
        });
    }

    u32 TextureBuffer::getTypeId(const TextureType &textureType) {
        u32 typeId;
        toGLTextureType(textureType, typeId);
        return typeId;
    }
}