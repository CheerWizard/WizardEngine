//
// Created by mecha on 28.08.2021.
//

#include "../../WizardEngine/src/Engine.h"

#include "TestLayer.h"

namespace test {

    class TestApplication : public engine::Application {

    public:

        TestApplication() {
            pushLayer(new TestLayer());
        }

        ~TestApplication() override = default;

    };

}

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating TestApplication...");
    return new test::TestApplication();
}
