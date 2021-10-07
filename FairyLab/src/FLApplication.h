//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "core/Application.h"

namespace fairy {

    class FLApplication : public engine::Application {
    protected:
        void onCreate() override;

        engine::WindowProps createWindowProps() override;
    };

}