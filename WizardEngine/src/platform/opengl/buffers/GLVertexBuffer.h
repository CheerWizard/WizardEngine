//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "../../../graphics/buffers/VertexBuffer.h"

namespace engine {

    class GLVertexBuffer : public VertexBuffer {
    public:
        void bind() override;
        void unbind() override;

        void load() override;

        void onCreate() override;

        void onDestroy() override;
    };

}