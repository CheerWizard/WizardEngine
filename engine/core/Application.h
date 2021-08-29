//
// Created by mecha on 28.08.2021.
//

#pragma once

namespace engine {

    class Application {

    public:
        Application();
        virtual ~Application();

    public:
        void run();

    };

    Application* createApplication();

}