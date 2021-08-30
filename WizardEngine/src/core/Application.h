//
// Created by mecha on 28.08.2021.
//

#include "Memory.h"
#include "Window.h"
#include "Logger.h"
#include "events/Event.h"

namespace engine {

    class Application : public EventCallback {

    public:
        Application();
        virtual ~Application();

    public:
        void run();
        void onEvent(Event& event) override;

    private:
        bool _isRunning = true;
        Scope<Window> _window;

    };

    Application* createApplication();

}