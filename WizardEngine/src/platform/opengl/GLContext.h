//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../graphics/GraphicsContext.h"

struct GLFWwindow;

namespace engine {

    class GLContext : public GraphicsContext {

    public:
        GLContext(void* nativeWindow);
        ~GLContext() override = default;

    public:
        void onCreate() override;
        void swapBuffers() override;
        Ref<GraphicsFactory> newGraphicsFactory() override;

    private:
        std::string getAPIName() override;
        const unsigned char *getVendorName() override;
        const unsigned char *getRendererName() override;
        const unsigned char *getVersion() override;

    private:
        GLFWwindow* _window;

    };

}