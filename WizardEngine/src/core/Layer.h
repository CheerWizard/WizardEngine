//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <string>

#include "Time.h"
#include "Logger.h"

#include "Events.h"

namespace engine {

    class Layer {

    public:

        explicit Layer(const std::string& tag = "Layer") : tag(tag) {}

        virtual ~Layer() {
            onDestroy();
        }

    public:
        virtual void onCreate();
        virtual void onUpdate(Time deltaTime);
        virtual void onDestroy();
        virtual void onImGuiRender();

    public:
        const std::string& getName() const { return tag; }

    protected:
        std::string tag;

    };

}
