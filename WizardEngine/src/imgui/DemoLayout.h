//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../core/Layout.h"

namespace engine {

    class DemoLayout : public Layout {

    public:
        DemoLayout() = default;
        ~DemoLayout() override = default;

    public:
        void onUpdate(Time deltaTime) override;
        void onKeyPressed(KeyCode keyCode) override;

    private:
        bool _show = false;

    };

}