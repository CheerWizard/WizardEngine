//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <string>

#include "Time.h"
#include "../events/Event.h"

namespace engine {

    class Layer {

    public:
        explicit Layer(const std::string& name = "Layer") : mName(name) {}
        virtual ~Layer() = default;

    public:
        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(Time deltaTime) {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}

    public:
        const std::string& getName() const { return mName; }

    protected:
        std::string mName;

    };

}
