//
// Created by mecha on 03.04.2022.
//

#include <core.h>
#include <ecs/ecs_test.h>

namespace test::ecs {

    struct TestComponent : Component<TestComponent> {
        f32 x = 0;
        f32 y = 0;
        bool flag = false;
        u32 i = 0;
    };

    void test_size(size_t expectedSize) {
        Registry registry;

        for (u32 i = 0 ; i < expectedSize ; i++) {
            registry.createEntity<TestComponent>();
        }
        assert_equals("test_size()", registry.size<TestComponent>(), expectedSize)
    }

    void test_componentTypesRegistration() {
        struct Test1 : Component<Test1> {};
        component(Test2, {
            float a;
            f32 b;
            u8 flag;
        })
        struct Test3 : Component<Test2> {};

        assert_equals("isValid<Test1>()",BaseComponent::isValid<Test1>(), true)
        assert_equals("isValid<Test2>()",BaseComponent::isValid<Test2>(), true)
        // this test will pass, even if Test3 is registered as Test2
        assert_equals("isValid<Test3>()",BaseComponent::isValid<Test3>(), true)
    }

    void test_entity() {
        component(Test, {})
        component(Test2, {})
        Registry registry;

        auto entity1 = registry.createEntity();
        assert_not_null("createEntity()", entity1);

        auto entity2 = registry.createEntity<Test>(Test {});
        void* test = registry.getComponent<Test>(entity2);
        assert_not_null("createEntity<Test>()", entity2);
        assert_not_null("addComponent<Test>(entity2)\ngetComponent<Test>(entity2)", test);

        registry.addComponent<Test2>(entity2);
        void* test2 = registry.getComponent<Test2>(entity2);
        assert_not_null("addComponent<Test2>(entity2)\ngetComponent<Test2>(entity2)", test2);

        entity1 = registry.deleteEntity(entity1);
        assert_null("deleteEntity(entity1)", entity1);
        entity2 = registry.deleteEntity(entity2);
        assert_null("deleteEntity(entity2)", entity2);
    }

    void test_suite() {
        test_size(100);
        test_componentTypesRegistration();
        test_entity();
    }
}