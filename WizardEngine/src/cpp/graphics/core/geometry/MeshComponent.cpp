//
// Created by mecha on 18.03.2022.
//

#include <graphics/core/geometry/MeshComponent.h>

namespace engine {

    void setInstanceId(MeshComponent &meshComponent, const uint32_t &instanceId) {
        const auto& meshInstanceId = meshComponent.meshes[0].vertexData.vertices[0].instanceId;
        if (meshInstanceId == instanceId) return;
        for (auto i = 0 ; i < meshComponent.meshCount ; i++) {
            const auto& vertexData = meshComponent.meshes[i].vertexData;
            for (auto j = 0; j < vertexData.vertexCount; j++) {
                auto& vertex = vertexData.vertices[j];
                vertex.instanceId = (float) instanceId;
            }
        }
    }

}