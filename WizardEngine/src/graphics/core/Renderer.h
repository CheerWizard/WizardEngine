//
// Created by mecha on 06.10.2021.
//

#pragma once

#include "RenderModel.h"

#include "../../ecs/Components.h"
#include "../../ecs/Family.h"

#include "geometry/MeshComponent.h"
#include "shader/BaseShader.h"
#include "commands/RenderCommands.h"

#define INSTANCE_COUNT_LIMIT 128

namespace engine {

    class Renderer {

    public:
        Renderer(const shader::BaseShaderProgram &shaderProgram) : shaderProgram(shaderProgram) {
            create();
        }

        ~Renderer() {
            release();
        }

    public:
        void render(const Entity& entity);
        void renderInstanced(MeshComponent &familyMesh, entt::registry &familyRegistry);
        void renderBatched(entt::registry &registry);

    public:
        void tryUploadMesh(
                MeshComponent& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );
        void tryUploadMesh(
                const uint32_t &instanceId,
                MeshComponent& meshComponent,
                uint32_t &previousVertexCount,
                uint32_t &previousIndexCount
        );
        void uploadMesh(MeshComponent &meshComponent);

    protected:
        void create();
        void begin();
        void end();
        void release();

    protected:
        RenderModel renderModel;
        // Shader API
        shader::BaseShaderProgram shaderProgram;
    };

}