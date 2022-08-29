//
// Created by mecha on 27.03.2022.
//

#pragma once

#include <io/Fonts.h>
#include <graphics/core/Renderer.h>
#include <graphics/text/Text.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    template<typename Text>
    class TextRenderer : public Renderer {

    public:
        TextRenderer() : Renderer() {}
        TextRenderer(const BaseShaderProgram& shaderProgram)
        : Renderer(shaderProgram, DrawType::QUAD, VERTEX) {
            init();
        }

    public:
        void render(ecs::Registry& registry) override;

    private:
        void init();
    };

    template<typename Text>
    void TextRenderer<Text>::init() {
        vRenderModels.emplace_back(createRenderModel(DEFAULT_VERTEX_COUNT));
    }

    template<typename Text>
    void TextRenderer<Text>::render(ecs::Registry &registry) {
        if (!shaderProgram.isReady() || registry.empty_entity() || registry.empty_components<Text>()) return;

        shaderProgram.start();
        shaderProgram.update(registry);

        uint32_t nextRenderModelId = 0;
        registry.each<Text>([this, &nextRenderModelId](Text* textComponent) {
            auto& text = *textComponent;
            auto& font = GET_FONT(text.bitmap.textureId);
            for (auto& c : text.text) {
                auto& character = font[c];
                auto& vertexDataComponent = character.vertexDataComponent;

//                vertexDataComponent.renderModelId += nextRenderModelId;
//                if (!validate<BatchVertex<CharVertex>>(vertexDataComponent)) {
//                    nextRenderModelId++;
//                }
            }
        });

        for (auto& renderModel : vRenderModels) {
            uint32_t totalVertexCount = 0;
            uint32_t i = 0;
            registry.each<Text>([this, &totalVertexCount, &i, &renderModel](Text* textComponent) {
                auto& text = *textComponent;
                const auto& vShader = shaderProgram.getVShader();
                const auto& fShader = shaderProgram.getFShader();

                vShader.setUniformArrayElement(i, text.transform.modelMatrix);
                fShader.setUniformArrayElement(i, text.color);
                fShader.setUniformArrayElement(i, text.transparency);
                fShader.setUniform(text.bitmap.sampler);
                TextureBuffer::bind(text.bitmap.textureId, text.bitmap.typeId);
                TextureBuffer::activate(text.bitmap.sampler.value);
                // no needs to update each character again, if the text didn't change!
                if (!text.isUpdated) {
                    i++;
                    totalVertexCount += text.text.length() * 4; // still need to update total vertex count to draw
                    return;
                }
                text.isUpdated = false;

                auto& font = GET_FONT(text.bitmap.textureId);
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
                        textY -= character.size.y() + text.paddingY;
                        continue;
                    }

                    float x = textX + character.bearing.x();
                    float y = textY + character.bearing.y() - character.size.y();
                    float w = character.size.x();
                    float h = character.size.y();

                    if (vertexDataComponent.isUpdated) {
                        auto& vertices = vertexDataComponent.vertexData.values;
                        vertices[3].vertex.position = { x, y };
                        vertices[2].vertex.position = { x + w, y };
                        vertices[1].vertex.position = { x + w, y + h };
                        vertices[0].vertex.position = { x, y + h };
                    }

                    renderModel.tryUploadBatch(i, vertexDataComponent, totalVertexCount);

                    textX += character.size.x() + text.paddingX;
                    previousChar = currentChar;
                }

                if (++i > shaderProgram.getInstancesPerDraw()) {
                    renderModel.vao.bind();
                    drawV(drawType, totalVertexCount);
                    i = 0;
                    totalVertexCount = 0;
                }
            });

            if (i > 0 && totalVertexCount > 0) {
                renderModel.vao.bind();
                drawV(drawType, totalVertexCount);
            }
            renderModel.resetCounts();
        }

        shaderProgram.stop();
    }

}
