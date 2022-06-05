//
// Created by mecha on 03.04.2022.
//

#pragma once

#include <ecs/Scene.h>

namespace test::ecs {
    // under testing
    using namespace engine::ecs;
    // tests
    void test_registry_size(size_t size);
    void test_componentTypesRegistration();
    void test_entity();
    void test_components();
    void test_scene();
    void test_serializeComponents();
    // test suites
    void test_suite();
}