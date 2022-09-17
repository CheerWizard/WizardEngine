//
// Created by mecha on 17.08.2022.
//

#pragma once

#include <core/Application.h>

namespace engine::network {

    using namespace core;

    class ENGINE_API NetworkApplication : Application {
        void onCreate() override;
        void onDestroy() override;
    };

}