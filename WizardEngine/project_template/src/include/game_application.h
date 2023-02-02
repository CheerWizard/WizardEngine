#pragma once

#include <core/Application.h>
#include <Launcher.h>

namespace game {

    class Application : public engine::core::Application {

    protected:
        void onCreate() override;
    };

}
