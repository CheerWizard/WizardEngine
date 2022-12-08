//
// Created by mecha on 25.09.2021.
//

#include <platform/graphics/TextureBuffer.h>
#include "glad/glad.h"

namespace engine::graphics {

    u32 ColorFormat::NONE = GL_NONE;

    int ColorFormat::R8 = GL_R8;

    int ColorFormat::RED_I32 = GL_R32I;

    u32 ColorFormat::RED_INTEGER = GL_RED_INTEGER;
    u32 ColorFormat::GREEN_INTEGER = GL_GREEN_INTEGER;
    u32 ColorFormat::BLUE_INTEGER = GL_BLUE_INTEGER;

    int ColorFormat::RED = GL_RED;
    int ColorFormat::GREEN = GL_GREEN;
    int ColorFormat::BLUE = GL_BLUE;

    u32 ColorFormat::U_RED = GL_RED;
    u32 ColorFormat::U_GREEN = GL_GREEN;
    u32 ColorFormat::U_BLUE = GL_BLUE;

    u32 ColorFormat::RGB = GL_RGB;
    u32 ColorFormat::RGBA = GL_RGBA;

    int ColorFormat::RGB8 = GL_RGB8;
    int ColorFormat::RGBA8 = GL_RGBA8;

    int ColorFormat::R16F = GL_R16F;
    int ColorFormat::RGB16F = GL_RGB16F;
    int ColorFormat::RGBA16F = GL_RGBA16F;

    int ColorFormat::RGB32F = GL_RGB32F;
    int ColorFormat::RGBA32F = GL_RGBA32F;

    int ColorFormat::SRGB = GL_SRGB;
    int ColorFormat::SRGBA = GL_SRGB_ALPHA;

    u32 TextureType::TEXTURE_2D = GL_TEXTURE_2D;
    u32 TextureType::CUBE_MAP = GL_TEXTURE_CUBE_MAP;
    u32 TextureType::TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE;
    u32 TextureType::TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY;

    u32 TextureFaceType::FRONT = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    u32 TextureFaceType::BACK = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    u32 TextureFaceType::LEFT = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    u32 TextureFaceType::RIGHT = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    u32 TextureFaceType::TOP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    u32 TextureFaceType::BOTTOM = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;

    u32 TextureParamName::MIN_FILTER = GL_TEXTURE_MIN_FILTER;
    u32 TextureParamName::MAG_FILTER = GL_TEXTURE_MAG_FILTER;
    u32 TextureParamName::WRAP_S = GL_TEXTURE_WRAP_S;
    u32 TextureParamName::WRAP_T = GL_TEXTURE_WRAP_T;
    u32 TextureParamName::WRAP_R = GL_TEXTURE_WRAP_R;

    int TextureParamValue::LINEAR = GL_LINEAR;
    int TextureParamValue::REPEAT = GL_REPEAT;
    int TextureParamValue::CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;

    u32 PixelsType::FLOAT = GL_FLOAT;
    u32 PixelsType::U_BYTE = GL_UNSIGNED_BYTE;
    u32 PixelsType::INT = GL_INT;
    u32 PixelsType::HALF_FLOAT = GL_HALF_FLOAT;

    u32 DepthStencilFormat::NONE = GL_NONE;
    u32 DepthStencilFormat::DEPTH24STENCIL8 = GL_DEPTH24_STENCIL8;
    u32 DepthStencilFormat::DEPTH32STENCIL8 = GL_DEPTH32F_STENCIL8;

    u32 DepthFormat::NONE = GL_NONE;
    int DepthFormat::DEPTH = GL_DEPTH_COMPONENT;
    u32 DepthFormat::U_DEPTH = GL_DEPTH_COMPONENT;
    u32 DepthFormat::DEPTH16 = GL_DEPTH_COMPONENT16;
    u32 DepthFormat::DEPTH24 = GL_DEPTH_COMPONENT24;
    u32 DepthFormat::DEPTH32 = GL_DEPTH_COMPONENT32;

    void TextureBuffer::create(u32 type) {
        this->type = type;
        glCreateTextures(type, 1, &id);
    }

    u32 TextureBuffer::createFrom(u32 type) {
        GLuint id;
        glCreateTextures(type, 1, &id);
        return id;
    }

    void TextureBuffer::destroy(u32 id) {
        glDeleteTextures(1, &id);
    }

    u32 TextureBuffer::recreate(u32 id, u32 type) {
        destroy(id);
        u32 newId;
        glCreateTextures(type, 1, &newId);
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
            glTextureParameteri(id, param.name, param.value);
        }
    }

    void TextureBuffer::bind(u32 id, u32 type) {
        glBindTexture(type, id);
    }

    void TextureBuffer::unbind(u32 type) {
        glBindTexture(type, 0);
    }

    void TextureBuffer::activate(u32 slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    u32 TextureBuffer::load(const char* filePath, io::Spectrum spectrum) {
        if (exists(filePath)) {
            return textureIdCache.at(filePath);
        }

        TextureBuffer textureBuffer;
        auto textureData = io::TextureFile::read(filePath, spectrum);
        if (textureData.pixels) {
            textureBuffer.create(TextureType::TEXTURE_2D);
            textureBuffer.bind();
            load(textureData);
            textureBuffer.unbind();
        } else {
            ENGINE_WARN("Can't load NULL pixels into texture!");
        }
        io::TextureFile::free(textureData.pixels);

        textureIdCache.insert(std::pair<const char*, u32>(filePath, textureBuffer.id));
        return textureBuffer.id;
    }

    u32 TextureBuffer::loadArray(const vector<std::string> &filepaths, io::Spectrum spectrum) {
        io::TextureArrayData textureArrayData = io::TextureFile::read(filepaths, spectrum);

        TextureBuffer textureBuffer(TextureType::TEXTURE_2D_ARRAY);
        textureBuffer.bind();
        loadArray(textureArrayData);
        textureBuffer.unbind();

        io::TextureFile::free(textureArrayData);

        return textureBuffer.id;
    }

    u32 TextureBuffer::load(const std::vector<TextureFace>& faces, io::Spectrum spectrum) {
        TextureBuffer textureBuffer(TextureType::CUBE_MAP);
        textureBuffer.bind();

        for (auto& face : faces) {
            auto textureData = io::TextureFile::read(face.filePath, spectrum);

            if (textureData.pixels) {
                loadFace(face.type, textureData);
            } else {
                ENGINE_WARN("Can't load NULL pixels into texture!");
            }

            io::TextureFile::free(textureData.pixels);
        }

        textureBuffer.unbind();
        return textureBuffer.id;
    }

    u32 TextureBuffer::generateCubeMap(
            int width, int height,
            int internalFormat, u32 dataFormat,
            u32 pixelsType
    ) {
        TextureBuffer textureBuffer(TextureType::CUBE_MAP);
        textureBuffer.bind();
        for (u32 i = 0 ; i < 6 ; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         internalFormat,
                         width, height, 0,
                         dataFormat,
                         pixelsType, nullptr);
        }
        textureBuffer.unbind();
        return textureBuffer.id;
    }

    void TextureBuffer::load(const io::TextureData &textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0,
                     textureData.internalFormat,
                     textureData.width, textureData.height, 0,
                     textureData.dataFormat,
                     textureData.pixelsType, textureData.pixels);
        if (textureData.spectrum == io::Spectrum::HDR) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    void TextureBuffer::loadArray(const io::TextureArrayData& textureArrayData) {
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,
                     textureArrayData.internalFormat,
                     textureArrayData.width, textureArrayData.height,
                     textureArrayData.textureData.size(), 0,
                     textureArrayData.dataFormat,
                     textureArrayData.pixelsType,
                     nullptr);

        for (int i = 0 ; i < textureArrayData.textureData.size() ; i++) {
            const auto& textureData = textureArrayData.textureData[i];
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i,
                            textureData.width, textureData.height, 1,
                            textureData.dataFormat,
                            textureData.pixelsType, textureData.pixels);
        }

        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void TextureBuffer::loadFace(u32 type, const io::TextureData &textureData) {
        glTexImage2D(type, 0,
                     textureData.internalFormat,
                     textureData.width, textureData.height,
                     0,
                     textureData.dataFormat,
                     textureData.pixelsType, textureData.pixels);
    }

    void TextureBuffer::disableByteAlignment() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    void TextureBuffer::setParams(u32 id, const std::vector<TextureParam>& params) {
        for (auto& param : params) {
            glTextureParameteri(id, param.name, param.value);
        }
    }

    void TextureBuffer::setDefaultParamsCubeMap(u32 id) {
        setParams(id, {
            { TextureParamName::MAG_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::MIN_FILTER, TextureParamValue::LINEAR },
            { TextureParamName::WRAP_S, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_T, TextureParamValue::CLAMP_TO_EDGE },
            { TextureParamName::WRAP_R, TextureParamValue::CLAMP_TO_EDGE },
        });
    }

    unordered_map<const char*, u32> TextureBuffer::textureIdCache;

    bool TextureBuffer::exists(const char *filepath) {
        return textureIdCache.find(filepath) != textureIdCache.end();
    }

    void TextureBuffer::clearTextureIdCache() {
        textureIdCache.clear();
    }

    u32 TextureBuffer::getTextureId(const char *filepath) {
        return textureIdCache.at(filepath);
    }

}