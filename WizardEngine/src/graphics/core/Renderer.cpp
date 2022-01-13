//
// Created by mecha on 06.10.2021.
//

#include "Renderer.h"

#include "../transform/TransformComponents.h"

namespace engine {

    void Renderer::create() {
        shaderProgram.bindVertexFormat();
        renderModel.vao.bind();
        auto& vbo = renderModel.vbos.front();
        auto& ibo = renderModel.ibos.front();
        vbo.setFormat(shaderProgram.getVertexFormat());
        ibo.bind();
        ibo.alloc();
        renderModel.vao.unbind();
    }

    void Renderer::renderInstanced(MeshComponent &familyMesh, entt::registry& familyRegistry) {
        begin();

        auto& vbo = renderModel.vbos.front();
        shaderProgram.update(familyRegistry);

        auto transforms = familyRegistry.view<Transform3dComponent>();
        // upload family mesh!
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        tryUploadMesh(familyMesh, totalVertexCount, totalIndexCount);
        // update each transform
        uint32_t i = 0;
        for (auto [entity, transform] : transforms.each()) {
            shaderProgram.getVShader().setUniformArrayElement(i++, transform);
            // if transform count is out of limit, then draw current instances and repeat iteration!
            if (i > INSTANCE_COUNT_LIMIT) {
                vbo.enableAttributes();
                drawTriangles(totalIndexCount, i);
                i = 0;
            }
        }

        // draw rest of instances!
        if (i > 0) {
            vbo.enableAttributes();
            drawTriangles(totalIndexCount, i);
        }

        end();
    }

    void Renderer::renderBatched(entt::registry &registry) {
        begin();

        auto& vbo = renderModel.vbos.front();
        shaderProgram.update(registry);

        auto entities = registry.group<Transform3dComponent, MeshComponent>();
        uint32_t totalVertexCount = 0;
        uint32_t totalIndexCount = 0;
        uint32_t i = 0;
        for (auto [entity, transform, mesh] : entities.each()) {
            tryUploadMesh(i, mesh, totalVertexCount, totalIndexCount);
            shaderProgram.getVShader().setUniformArrayElement(i++, transform);
            if (i > INSTANCE_COUNT_LIMIT) {
                vbo.enableAttributes();
                drawTriangles(totalIndexCount);
                i = 0;
                totalVertexCount = 0;
                totalIndexCount = 0;
            }
        }

        if (i > 0) {
            vbo.enableAttributes();
            drawTriangles(totalIndexCount);
        }

        end();
    }

    void Renderer::render(const Entity &entity) {
        begin();

        shaderProgram.update(entity);

        uint32_t totalIndexCount = 0;
        uint32_t totalVertexCount = 0;
        // try to upload mesh
        auto mesh = entity.getPtr<MeshComponent>();
        if (mesh) {
            tryUploadMesh(*mesh, totalVertexCount, totalIndexCount);
        }
        // try to upload transform
        auto transform = entity.getPtr<Transform3dComponent>();
        if (transform) {
            shaderProgram.getVShader().setUniform(*transform);
        }

        renderModel.vbos.front().enableAttributes();
        drawTriangles(totalIndexCount);

        end();
    }

    void Renderer::uploadMesh(MeshComponent &meshComponent) {
        auto& vbo = renderModel.vbos.front();
        auto& ibo = renderModel.ibos.front();

        vbo.bind();
        ibo.bind();
        const auto& meshes = meshComponent.meshes;
        for (auto i = 0; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshes[i].vertexData;
            vbo.load(vertexData);
            const auto& indexData = meshes[i].indexData;
            ibo.load(indexData);
        }
    }

    void Renderer::tryUploadMesh(
            MeshComponent& meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        uint32_t instanceID = 0;
        tryUploadMesh(instanceID, meshComponent, previousVertexCount, previousIndexCount);
    }

    void Renderer::tryUploadMesh(
            const uint32_t &instanceId,
            MeshComponent &meshComponent,
            uint32_t &previousVertexCount,
            uint32_t &previousIndexCount
    ) {
        if (meshComponent.isUpdated) {
            meshComponent.isUpdated = false;
            updateStart(meshComponent, previousVertexCount, previousIndexCount);
            setInstanceId(meshComponent, instanceId);
            updateCounts(meshComponent);
            uploadMesh(meshComponent);
        }

        previousIndexCount += meshComponent.totalIndexCount;
        previousVertexCount += meshComponent.totalVertexCount;
    }

    void Renderer::begin() {
        shaderProgram.start();
        renderModel.vao.bind();
    }

    void Renderer::end() {
        renderModel.vao.unbind();
        shaderProgram.stop();
    }

    void Renderer::release() {
        engine::release(renderModel);
        shaderProgram.release();
    }

}