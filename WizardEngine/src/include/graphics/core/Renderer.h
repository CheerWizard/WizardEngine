//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "RenderModel.h"

#include <ecs/Components.h>

#include "geometry/MeshComponent.h"
#include "shader/BaseShader.h"

#include <platform/graphics/RenderCommands.h>

#define INSTANCE_COUNT_LIMIT 128

using namespace engine::shader;

namespace engine {

    class Renderer {

    public:
        Renderer(const Ref<BaseShaderProgram>& shaderProgram) : shaderProgram(shaderProgram) {
            create();
        }

        ~Renderer() {
            release();
        }

    public:
        void render(const Entity& entity);
        void renderInstanced(entt::registry &registry);
        void renderBatched(entt::registry &registry);

    public:
        void tryUploadMesh(
                MeshComponent& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                RenderModel& renderModel
        );
        void tryUploadMesh(
                const uint32_t &instanceId,
                MeshComponent& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount,
                RenderModel& renderModel
        );
        void uploadMesh(MeshComponent &meshComponent, RenderModel& renderModel);

    protected:
        void create();
        RenderModel& createRenderModel(const uint32_t& vertexCount, const uint32_t& indexCount);
        void release();
        bool validate(RenderModel& renderModel, MeshComponent& meshComponent);

    protected:
        std::vector<RenderModel> renderModels;
        Ref<BaseShaderProgram> shaderProgram;
    };

}