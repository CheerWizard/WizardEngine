//
// Created by mecha on 09.01.2022.
//

#include <graphics/core/RenderModel.h>

namespace engine {

    void release(RenderModel& renderModel) {
        renderModel.vao.destroy();
        renderModel.vbo.destroy();
        renderModel.ibo.destroy();
    }

    void resetCounts(RenderModel& renderModel) {
        renderModel.vbo.setCount(0);
        renderModel.ibo.setCount(0);
    }

    void VertexBuffer::setFormat(const shader::VertexFormat &vertexFormat) {
        setFormat(vertexFormat, totalCount);
    }

    void VertexBuffer::setFormat(const shader::VertexFormat &vertexFormat, const uint32_t &vertexCount) {
        this->vertexFormat = vertexFormat;
        bind();
        alloc(vertexCount);
        bindAttributes();
    }

    void VertexBuffer::recreate() {
        destroy();
        create();
    }

    void VertexBuffer::alloc(const uint32_t &vertexCount) {
        count = vertexCount;
        size_t size = vertexFormat.getSize() * vertexCount;
        malloc(size);
    }

    void VertexBuffer::alloc() {
        alloc(totalCount);
    }
}
