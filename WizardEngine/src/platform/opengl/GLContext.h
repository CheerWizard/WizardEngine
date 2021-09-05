//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../graphics/GraphicsContext.h"

#include "renderer/GLRenderer.h"

struct GLFWwindow;

namespace engine {

    class GLContext : public GraphicsContext {

    public:
        explicit GLContext(GLFWwindow* window);
        ~GLContext() override = default;

    public:
        void onCreate() override;
        void swapBuffers() override;
        void clearDisplay() override;

        Renderer* newRenderer() override;

    private:
        std::string getAPIName() override;
        const unsigned char *getVendor() override;
        const unsigned char *getRenderer() override;
        const unsigned char *getVersion() override;

    private:
        GLFWwindow* _window;

    };

}