//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/Memory.h"
#include "../core/Assert.h"
#include "render/Renderer.h"

#include <string>

namespace engine {

    class GraphicsContext {

    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

    public:
        void printInfo();

    public:
        virtual void onCreate() = 0;
        virtual void swapBuffers() = 0;
        virtual void clearDisplay() = 0;

        virtual Renderer* newRenderer() = 0;

    private:
        virtual std::string getAPIName() = 0;
        virtual const unsigned char* getVendor() = 0;
        virtual const unsigned char* getRenderer() = 0;
        virtual const unsigned char* getVersion() = 0;

    };

}