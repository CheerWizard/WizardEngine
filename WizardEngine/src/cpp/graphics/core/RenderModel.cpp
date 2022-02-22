//
// Created by mecha on 09.01.2022.
//

#include <graphics/core/RenderModel.h>

namespace engine {

    void release(RenderModel& renderModel) {
        renderModel.vao.destroy();
        for (auto& vbo : renderModel.vbos) {
            vbo.destroy();
        }
        for (auto& ibo : renderModel.ibos) {
            ibo.destroy();
        }
    }

}