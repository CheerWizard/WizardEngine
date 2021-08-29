//
// Created by mecha on 28.08.2021.
//

/**
 * Test app, used to extend all components of Engine.h sources and test it.
 * */

#include "../engine/Engine.h"

class TestApplication : public engine::Application {

public:
    TestApplication();
    ~TestApplication() override;

};
