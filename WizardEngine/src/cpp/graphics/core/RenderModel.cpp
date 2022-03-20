//
// Created by mecha on 09.01.2022.
//

#include <graphics/core/RenderModel.h>

namespace engine {

    void release(VRenderModel& renderModel) {
        renderModel.vao.destroy();
        renderModel.vbo.destroy();
    }

    void resetCounts(VRenderModel& renderModel) {
        renderModel.vbo.setCount(0);
    }

    void release(VIRenderModel& renderModel) {
        renderModel.vao.destroy();
        renderModel.vbo.destroy();
        renderModel.ibo.destroy();
    }

    void resetCounts(VIRenderModel& renderModel) {
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
        totalCount = vertexCount;
        size_t size = vertexFormat.getSize() * vertexCount;
        malloc(size);
    }

    void VertexBuffer::alloc() {
        alloc(totalCount);
    }
}
