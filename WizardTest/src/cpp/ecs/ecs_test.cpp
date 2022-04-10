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

    void test_registry_size(size_t expectedSize) {
        Registry registry;

        for (u32 i = 0 ; i < expectedSize ; i++) {
            registry.createEntity<TestComponent>();
        }

        assert_equals("test_entity_count()", registry.entity_count(), expectedSize)
        assert_equals("test_component_count<TestComponent>()", registry.component_count<TestComponent>(), expectedSize)
    }

    void test_componentTypesRegistration() {
        empty_component(Test1)
        component(Test2) {
            float a;
            f32 b;
            u8 flag;
        };
        struct Test3 : Component<Test2> {};

        assert_equals("isValid<Test1>()",BaseComponent::isValid<Test1>(), true)
        assert_equals("isValid<Test2>()",BaseComponent::isValid<Test2>(), true)
        // this test will pass, even if Test3 is registered as Test2
        assert_equals("isValid<Test3>()",BaseComponent::isValid<Test3>(), true)
    }

    void test_entity() {
        empty_component(Test)
        empty_component(Test2);
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

        registry.deleteEntity(entity1);
        assert_null("deleteEntity(entity1)", entity1);
        registry.deleteEntity(entity2);
        assert_null("deleteEntity(entity2)", entity2);
    }

    void test_components() {
        component(Position) {
            f32 x = 0;
            f32 y = 0;
            Position(const f32& x, const f32& y)
            : x(x), y(y) {}
        };

        component(Color) {
            f32 r = 0;
            f32 g = 0;
            f32 b = 0;
            f32 a = 0;
            Color(const f32& r, const f32& g, const f32& b, const f32& a)
            : r(r), g(g), b(b), a(a) {}
        };

        component(Vertex) {
            Position position;
            Color color;
            Vertex(const Position& position, const Color& color)
            : position(position), color(color) {}
        };

        component(Polygon) {
            Vertex* vertices = nullptr;
            size_t vertexCount = 0;
            Polygon(Vertex* vertices, const size_t& vertexCount)
            : vertices(vertices), vertexCount(vertexCount) {}
        };

        component(Mesh) {
            Polygon polygon;
            u32* indices = nullptr;
            size_t indexCount = 0;
            Mesh(const Polygon& polygon, u32* indices, const size_t& indexCount)
            : polygon(polygon), indices(indices), indexCount(indexCount) {}
        };

        Registry registry;

        for (u32 i = 0 ; i < 10 ; i++) {
            entity_id entityId = registry.createEntity<Polygon>(Polygon {
                    new Vertex[4] {
                            Vertex { { 0, 0 }, { 0, 0, 1, 1 } },
                            Vertex { { 0, 1 }, { 0, 1, 0, 1 } },
                            Vertex { { 1, 1 }, { 1, 0, 0, 1 } },
                            Vertex { { 1, 0 }, { 1, 1, 1, 1 } }
                    },
                    4
            });
            registry.addComponent<Mesh>(entityId, Mesh {
                    Polygon {
                            new Vertex[8]{
                                    Vertex{{0, 0},
                                           {0, 0, 1, 1}},
                                    Vertex{{0, 1},
                                           {0, 1, 0, 1}},
                                    Vertex{{1, 1},
                                           {1, 0, 0, 1}},
                                    Vertex{{1, 0},
                                           {1, 1, 1, 1}},
                                    Vertex{{0, 0},
                                           {0, 0, 1, 1}},
                                    Vertex{{0, 1},
                                           {0, 1, 0, 1}},
                                    Vertex{{1, 1},
                                           {1, 0, 0, 1}},
                                    Vertex{{1, 0},
                                           {1, 1, 1, 1}}
                            },
                            8
                    },
                    new u32[8] {
                            0, 1, 2, 3, 4, 5, 6, 7
                    },
                    8
            });
        }

        for (u32 i = 0 ; i < 10 ; i++) {
            registry.createEntity<Mesh>(Mesh {
                    Polygon {
                            new Vertex[8]{
                                    Vertex{{0, 0},
                                           {0, 0, 1, 1}},
                                    Vertex{{0, 1},
                                           {0, 1, 0, 1}},
                                    Vertex{{1, 1},
                                           {1, 0, 0, 1}},
                                    Vertex{{1, 0},
                                           {1, 1, 1, 1}},
                                    Vertex{{0, 0},
                                           {0, 0, 1, 1}},
                                    Vertex{{0, 1},
                                           {0, 1, 0, 1}},
                                    Vertex{{1, 1},
                                           {1, 0, 0, 1}},
                                    Vertex{{1, 0},
                                           {1, 1, 1, 1}}
                            },
                            8
                    },
                    new u32[8] {
                            0, 1, 2, 3, 4, 5, 6, 7
                    },
                    8
            });
        }

        assert_equals("test_entity_count()", registry.entity_count(), 20)
        assert_equals("test_component_count<Polygon>()", registry.component_count<Polygon>(), 10)
        assert_equals("test_component_count<Mesh>()", registry.component_count<Polygon>(), 10)

        u32 i = 0;
        registry.each<Polygon>([&i](Polygon* polygon) {
            i++;
            assert_not_null("each<Polygon>()", (void*) polygon);
        });
        assert_equals("registry.each<Polygon>() elements count", i, 10);

        i = 0;
        registry.each<Mesh>([&i](Mesh* mesh) {
            i++;
            assert_not_null("each<Mesh>()", (void*) mesh);
        });
        assert_equals("registry.each<Mesh>() elements count", i, 20);

        i = 0;
        registry.each<Polygon, Mesh>([&i](Polygon* polygon, Mesh* mesh) {
            i++;
            assert_not_null("each<Polygon, Mesh>()", (void*) polygon);
            assert_not_null("each<Polygon, Mesh>()", (void*) mesh);
        });
        assert_equals("registry.each<Polygon, Mesh>() elements count", i, 10);
    }

    void test_suite() {
        test_registry_size(100);
        test_componentTypesRegistration();
        test_entity();
        test_components();
    }
}