//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/String.h"
#include "GraphicsFactory.h"

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
        virtual Ref<GraphicsFactory> newGraphicsFactory() = 0;

    private:
        virtual std::string getAPIName() = 0;
        virtual const unsigned char* getVendorName() = 0;
        virtual const unsigned char* getRendererName() = 0;
        virtual const unsigned char* getVersion() = 0;

    };

}