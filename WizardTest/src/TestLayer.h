//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "../../WizardEngine/src/core/Layer.h"
#include "../../WizardEngine/src/core/Logger.h"

namespace test {

    class TestLayer : public engine::Layer {

    public:
        TestLayer() : Layer("TestLayer") {

        }

    public:

        void onAttach() override {
            CLIENT_INFO("{0} : onAttach()", mName);
        }

        void onDetach() override {
            CLIENT_INFO("{0} : onDetach()", mName);
        }

        void onUpdate(engine::Time deltaTime) override {
            CLIENT_INFO("{0} : onUpdate()", mName);
            CLIENT_INFO("Delta time : {0} ms", deltaTime.getMilliseconds());
        }

        void onEvent(engine::Event& event) override {
            CLIENT_INFO("{0} : onEvent()", mName);
            CLIENT_INFO("{0}", event.toString());
        }

    };

}

