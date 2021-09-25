//
// Created by mecha on 12.09.2021.
//

#pragma once

#include "vector"
#include "string"

#include "geometry/Vertex.h"
#include "../core/Memory.h"

#include "buffers/IndexData.h"

#include "../math/ViewProjectionMatrix.h"
#include "../math/Uniform.h"

#include "buffers/TextureData.h"

namespace engine {

    struct GraphicsObject {
        std::string shaderName;
        VertexData vertexData;
        IndexData indexData;

        bool isUpdated = false; //used to notify RenderSystem that this struct data needs to be uploaded to appropriate GPU buffer.

        //todo add ECS
        PerspectiveMatrix* perspectiveProjection = nullptr;
        OrthographicMatrix* orthographicProjection = nullptr;
        TransformMatrix* transform = nullptr;

        FloatUniform* brightness = nullptr;
        TextureSampler* textureSampler = nullptr;

        GraphicsObject(const std::string &shaderName) : shaderName(shaderName) {
        }

        GraphicsObject(
                const std::string &shaderName,
                const VertexData &vertexData,
                const IndexData &indexData
                ) :
                shaderName(shaderName),
                vertexData(vertexData),
                indexData(indexData) {}
    };

}