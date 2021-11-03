//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "../../graphics/GraphicsModule.h"

namespace engine {

    class GLModule : public GraphicsModule {

    public:
        GLModule() = default;
        ~GLModule() override = default;

    public:
        void createContext(void* nativeWindow) override;
        Ref<GraphicsFactory> newGraphicsFactory() override;

    };

}