//
// Created by mecha on 05.04.2022.
//

#pragma once

#include <core/Application.h>
#include <Launcher.h>

namespace test {

    class Application : public engine::core::Application {

    protected:
        void onCreate() override;
        engine::core::WindowProps createWindowProps() override;

    private:
        void createTest();
    };

}
