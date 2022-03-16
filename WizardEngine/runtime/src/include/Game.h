//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <core/Application.h>

namespace game {

    class Application : public engine::Application {
    protected:
        engine::WindowProps createWindowProps() override;
    };

}