//
// Created by mecha on 27.03.2022.
//

#pragma once

#include <core/Fonts.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/sources/TextureSource.h>
#include <graphics/text/Text.h>
#include <graphics/camera/CameraComponents.h>

namespace engine {

    class TextRenderer : public Renderer {

    public:
        TextRenderer(const Ref<BaseShaderProgram>& shaderProgram)
        : Renderer(shaderProgram, QUAD, VERTEX) {
            init();
        }

    public:
        template<typename Text>
        void render(entt::registry& registry);

    private:
        void init();
    };

    template<typename Text>
    void TextRenderer::render(entt::registry &registry) {
        if (!shaderProgram->isReady()) return;

        auto entities = registry.view<Text>();
        if (entities.empty()) return; // nothing to render

        shaderProgram->start();
        shaderProgram->update(registry);
        const auto& vShader = shaderProgram->getVShader();
        const auto& fShader = shaderProgram->getFShader();

        uint32_t nextRenderModelId = 0;
        for (auto [entity, text] : entities.each()) {
            if (FONT_ABSENT(text.font)) continue;

            auto& font = GET_FONT(text.font);
            for (auto& c : text.text) {
                auto& character = font[c];
                auto& vertexDataComponent = character.vertexDataComponent;

                vertexDataComponent.renderModelId += nextRenderModelId;
                auto& renderModel = vRenderModels[vertexDataComponent.renderModelId];
                if (!validate<BatchVertex<CharVertex>>(renderModel, vertexDataComponent)) {
                    nextRenderModelId++;
                }
            }
        }

        for (auto& renderModel : vRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t i = 0;
            for (auto [entity, text] : entities.each()) {
                // skip full text rendering, if font is absent in memory for this text
                if (FONT_ABSENT(text.font)) {
                    i++;
                    continue;
                }

                vShader.setUniformArrayElement(i, text.transform);
                fShader.setUniformArrayElement(i, text.color);
                fShader.setUniformArrayElement(i, text.transparency);
                fShader.setUniform(text.bitmap.sampler);
                ACTIVATE_TEXTURE_PATH(text.bitmap, "assets/bitmaps");

                // no needs to update each character again, if the text didn't change!
                if (!text.isUpdated) {
                    i++;
                    totalVertexCount += text.text.length() * 4; // need this to redraw same text, without changes
                    continue;
                }
                text.isUpdated = false;

                auto& font = GET_FONT(text.font);
                float textX = 0;
                float textY = 0;
                char previousChar = 0;
                for (auto& currentChar : text.text) {
                    int currentCode = int((uint8_t) currentChar);
                    int previousCode = int((uint8_t) previousChar);
                    ENGINE_INFO("Character[code : {0}, value : {1}]", currentCode, currentChar);
                    Character character = font[currentChar];
                    auto vertexDataComponent = character.vertexDataComponent;

                    // todo can be optimized or completely removed!
                    if (renderModel.id != vertexDataComponent.renderModelId) {
                        i++; // shift instance id
                        continue;
                    }

                    // if char is a whitespace - we should add its width
                    if (currentCode == 32) {
                        textX += text.whiteSpaceWidth;
                        continue;
                    }

                    // if combination of chars is \n - we should add new line
                    if (previousCode == 92 && currentChar == 'n') {
                        textX = 0;
                        textY -= character.size.y + text.paddingY;
                        continue;
                    }

                    float x = textX + character.bearing.x;
                    float y = textY + character.bearing.y - character.size.y;
                    float w = character.size.x;
                    float h = character.size.y;

                    if (vertexDataComponent.isUpdated) {
                        auto& vertices = vertexDataComponent.vertexData.vertices;
                        vertices[3].vertex.position = { x, y };
                        vertices[2].vertex.position = { x + w, y };
                        vertices[1].vertex.position = { x + w, y + h };
                        vertices[0].vertex.position = { x, y + h };
                    }

                    tryUploadBatch(i, vertexDataComponent, totalVertexCount, renderModel);

                    textX += character.size.x + text.paddingX;
                    previousChar = currentChar;
                }

                if (++i > INSTANCE_COUNT_LIMIT) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount);
                    i = 0;
                    totalVertexCount = 0;
                }
            }

            if (i > 0 && totalVertexCount > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount);
            }
            resetCounts(renderModel);
        }

        shaderProgram->stop();
    }

}
