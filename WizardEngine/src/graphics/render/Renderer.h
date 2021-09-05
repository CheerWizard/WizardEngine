//
// Created by mecha on 05.09.2021.
//

#pragma once

namespace engine {

    class Renderer {
    public:
        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;
        virtual void onDestroy() = 0;
    };

}