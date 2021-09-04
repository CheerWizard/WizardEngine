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

        void onCreate() override {
            CLIENT_INFO("{0} : onCreate()", tag);
        }

        void onDestroy() override {
            CLIENT_INFO("{0} : onDestroy()", tag);
        }

        void onUpdate(engine::Time deltaTime) override {
            CLIENT_INFO("{0} : onUpdate()", tag);
            CLIENT_INFO("Delta time : {0} ms", deltaTime.getMilliseconds());
        }

    };

}

