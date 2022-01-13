//
// Created by mecha on 09.01.2022.
//

#pragma once

#include "../../platform/includes/graphics/vao.h"
#include "../../platform/includes/graphics/vbo.h"
#include "../../platform/includes/graphics/ibo.h"

namespace engine {

    struct RenderModel {
        VertexArray vao;
        std::vector<VertexBuffer> vbos = { VertexBuffer() };
        std::vector<IndexBuffer> ibos = { IndexBuffer() };
    };

    void release(RenderModel& renderModel);

}