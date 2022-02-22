//
// Created by mecha on 09.01.2022.
//

#pragma once

#include <platform/graphics/VertexArray.h>
#include <platform/graphics/VertexBuffer.h>
#include <platform/graphics/IndexBuffer.h>

namespace engine {

    struct RenderModel {
        VertexArray vao;
        std::vector<VertexBuffer> vbos = { VertexBuffer() };
        std::vector<IndexBuffer> ibos = { IndexBuffer() };
    };

    void release(RenderModel& renderModel);

}