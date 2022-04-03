//
// Created by mecha on 03.04.2022.
//

#include <ecs/ecs_test.h>

namespace test::ecs {

    struct TestComponent : Component<TestComponent> {
        f32 x = 0;
        f32 y = 0;
        bool flag = false;
        u32 i = 0;
    };

    void test() {
        Registry registry;
        TestComponent test;

        // entities
        entity_id entityId = registry.createEntity();
        entity_id entityId2 = registry.createEntity<TestComponent>(TestComponent {});
        entity_id entityId3 = registry.createEntity<TestComponent>(test);
        registry.deleteEntity(entityId3);

        // components
        registry.addComponent<TestComponent>(entityId, test);
        registry.addComponent<TestComponent>(entityId, TestComponent {});
        auto* testPtr = registry.getComponent<TestComponent>(entityId);
        registry.removeComponent<TestComponent>(entityId);

        // iterations
        registry.read<TestComponent>([](const TestComponent* tc) {
        });
        registry.write<TestComponent>([](TestComponent* tc) {
        });

        registry.deleteEntity(entityId);

        RUNTIME_INFO("test passed!");
    }

    void test_suite() {
        test();
    }
}