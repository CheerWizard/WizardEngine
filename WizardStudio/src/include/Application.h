//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <core/Application.h>

namespace studio {

    class Application : public engine::Application {
    protected:
        void onCreate() override;
        engine::WindowProps createWindowProps() override;

    private:
        void createTest();
    };

}