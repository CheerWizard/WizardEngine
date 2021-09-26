//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

namespace engine {

    struct ShapeComponent {
        VertexData vertexData;
        IndexData indexData;

        bool isUpdated = false; //used to notify RenderSystem that this struct data needs to be uploaded to appropriate GPU buffer.

        ShapeComponent() = default;

        ShapeComponent(const VertexData &vertexData,
                       const IndexData &indexData) :
                       vertexData(vertexData),
                       indexData(indexData) {}

    public:
        void applyChanges() {
            isUpdated = true;
        }

    };

}