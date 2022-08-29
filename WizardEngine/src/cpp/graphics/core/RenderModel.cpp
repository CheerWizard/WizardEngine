//
// Created by mecha on 09.01.2022.
//

#include <graphics/core/RenderModel.h>

namespace engine::graphics {

    void VRenderModel::release() {
        vao.destroy();
        vbo.destroy();
    }

    void VRenderModel::resetCounts() {
        vbo.setCount(0);
    }

    void VIRenderModel::release() {
        vao.destroy();
        vbo.destroy();
        ibo.destroy();
    }

    void VIRenderModel::resetCounts() {
        vbo.setCount(0);
        ibo.setCount(0);
    }

    void VertexBuffer::setFormat(const shader::VertexFormat &vertexFormat) {
        setFormat(vertexFormat, totalCount);
    }

    void VertexBuffer::setFormat(const shader::VertexFormat &vertexFormat, const u32 &vertexCount) {
        this->vertexFormat = vertexFormat;
        bind();
        allocDynamic(vertexCount);
        bindAttributes();
    }

    void VertexBuffer::recreate() {
        destroy();
        create();
    }

    void VertexBuffer::allocDynamic(const u32 &vertexCount) {
        totalCount = vertexCount;
        size_t size = vertexFormat.getSize() * vertexCount;
        mallocDynamic(size);
    }

    void VertexBuffer::allocDynamic() {
        allocDynamic(totalCount);
    }
}
