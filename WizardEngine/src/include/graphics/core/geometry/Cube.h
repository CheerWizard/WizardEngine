//
// Created by mecha on 26.03.2022.
//

#pragma once

#include <graphics/core/buffer_data/VertexData.h>

namespace engine::graphics {

    template<typename T>
    struct Cube : VertexDataComponent<T> {

        Cube() {
            init();
        }

        void init() {
            auto vertices = new T[36] {
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{1.0f,  1.0f, -1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{1.0f,  1.0f,  1.0f}},
                    {{-1.0f,  1.0f,  1.0f}},
                    {{-1.0f,  1.0f, -1.0f}},
                    {{-1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{1.0f, -1.0f, -1.0f}},
                    {{-1.0f, -1.0f,  1.0f}},
                    {{1.0f, -1.0f,  1.0f}},
            };

            this->vertexData = { vertices, 0, 36 };
            this->drawType = DrawType::TRIANGLE;
        }
    };
}
