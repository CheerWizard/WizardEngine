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

    bool hasCapacity(const RenderModel& renderModel, const MeshComponent &meshComponent) {
        bool hasVertexCapacity = renderModel.vbo.hasCapacity(meshComponent.totalVertexCount);
        bool hasIndexCapacity = renderModel.ibo.hasCapacity(meshComponent.totalIndexCount);
        return hasVertexCapacity && hasIndexCapacity;
    }

    void resetCounts(RenderModel& renderModel) {
        renderModel.vbo.setCount(0);
        renderModel.ibo.setCount(0);
    }

    void increaseCounts(RenderModel& renderModel, const MeshComponent &meshComponent) {
        renderModel.vbo.increaseCount(meshComponent.totalVertexCount);
        renderModel.ibo.increaseCount(meshComponent.totalIndexCount);
    }

}