//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/render/Renderer.h"

namespace engine {

    class GLRenderer : public Renderer {

    public:
        GLRenderer() = default;
        ~GLRenderer() = default;

    public:
        void onCreate() override;
        void onUpdate() override;
        void onDestroy() override;

    private:
        unsigned int _vertexArray, _vertexBuffer, _indexBuffer;

    };

}