//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "../buffers/VertexData.h"
#include "../buffers/IndexData.h"

namespace engine {

    enum ShapePrimitive : unsigned char {
        TRIANGLE = 0,
        QUAD = 1
    };

    struct ShapeComponent {
        VertexData vertexData;
        IndexData indexData;
        ShapePrimitive primitive;

        bool isUpdated = false; //used to notify RenderSystem that this Component has been updated.

        ShapeComponent() = default;

        ShapeComponent(const VertexData &vertexData,
                       const IndexData &indexData,
                       const ShapePrimitive &shapePrimitive
                       ) :
                       vertexData(vertexData),
                       indexData(indexData) {}

    public:
        void applyChanges() {
            isUpdated = true;
        }

    public:
        ShapeComponent copy() const {
            return ShapeComponent {
                vertexData.copy(),
                indexData.copy(),
                primitive
            };
        }

    };

}