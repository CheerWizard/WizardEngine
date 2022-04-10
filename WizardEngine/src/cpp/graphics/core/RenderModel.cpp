//
// Created by mecha on 09.01.2022.
//

#include <graphics/core/RenderModel.h>

namespace engine::graphics {

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
