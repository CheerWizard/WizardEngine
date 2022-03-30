//
// Created by mecha on 27.03.2022.
//

#include <graphics/text/TextRenderer.h>
#include <graphics/text/Text.h>
#include <core/Fonts.h>
#include <graphics/core/sources/TextureSource.h>

namespace engine {

    void TextRenderer::render(entt::registry &registry) {
        if (!shaderProgram->isReady()) return;

        auto entities = registry.view<TextComponent>();
        if (entities.empty()) return; // nothing to render

        shaderProgram->start();
        shaderProgram->update(registry);

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
                if (FONT_ABSENT(text.font)) continue;

                shaderProgram->getVShader().setUniformArrayElement(i, text.transform);
                shaderProgram->getFShader().setUniformArrayElement(i, text.color);
                shaderProgram->getFShader().setUniform(text.bitmap.sampler);
                ACTIVATE_TEXTURE_PATH(text.bitmap, "assets/bitmaps");

                auto& font = GET_FONT(text.font);
                float textX = 0;
                for (auto& c : text.text) {
                    int code = int((uint8_t) c);
                    ENGINE_INFO("Character[code : {0}, value : {1}]", code, c);
                    Character character = font[c];
                    auto vertexDataComponent = character.vertexDataComponent;

                    if (renderModel.id != vertexDataComponent.renderModelId) {
                        i++; // shift instance id
                        continue;
                    }

                    // is whitespace char
                    if (code == 32) {
                        textX += text.whiteSpaceWidth;
                        continue;
                    }

                    float x = textX + character.bearing.x;
                    float y = character.bearing.y - character.size.y;
                    float w = character.size.x;
                    float h = character.size.y;

                    if (vertexDataComponent.isUpdated) {
                        auto& vertices = vertexDataComponent.vertexData.vertices;
                        vertices[3].vertex.position = { x, y };
                        vertices[2].vertex.position = { x + w, y };
                        vertices[1].vertex.position = { x + w, y + h };
                        vertices[0].vertex.position = { x, y + h };
                    }
                    textX += character.size.x + text.padding;

                    tryUploadBatch(i, vertexDataComponent, totalVertexCount, renderModel);
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

    void TextRenderer::init() {
        createRenderModel(DEFAULT_VERTEX_COUNT);
    }

}