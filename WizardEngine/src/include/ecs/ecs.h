//
// Created by mecha on 02.04.2022.
//

#pragma once

#include <core/identifier.h>
#include <core/vector.h>
#include <core/map.h>
#include <core/Time.h>
#include <core/immutable.h>
#include <tuple>

namespace engine::ecs {

    using namespace core;
    // type identifiers for entities and components
    #define invalid_entity_id nullptr
    typedef void* entity_id;
    typedef u32 component_id;
    typedef size_t component_size;
    typedef vector<u8> component_data; // contagious component storage of the same type!

    struct BaseComponent;
    typedef u32 (*ComponentCreateFunction)(component_data& data, entity_id entityId, BaseComponent* component);
    typedef void (*ComponentDestroyFunction)(BaseComponent* component);

    // dynamic component type system
    typedef std::tuple<ComponentCreateFunction, ComponentDestroyFunction, component_size> ComponentType;
    struct BaseComponent {
        entity_id entityId = invalid_entity_id;

    public:
        static component_id registerComponentType(
                ComponentCreateFunction createFunction,
                ComponentDestroyFunction destroyFunction,
                component_size size
        );

    public:
        inline static const ComponentType& getType(component_id id) {
            return componentTypes[id];
        }

        inline static ComponentCreateFunction getCreateFunction(component_id id) {
            return std::get<0>(componentTypes[id]);
        }

        inline static ComponentDestroyFunction getDestroyFunction(component_id id) {
            return std::get<1>(componentTypes[id]);
        }

        inline static component_size getSize(component_id id) {
            return std::get<2>(componentTypes[id]);
        }

        inline static bool isValid(component_id id) {
            return id >= componentTypes.size();
        }

        template<class Component>
        inline static bool isValid() {
            return isValid(Component::ID);
        }

    private:
        static vector<ComponentType> componentTypes;
    };

    // Component

    // typed component, which will set type id, size and create/destroy function.
    // extend Component struct, provide your type, and that's all you need.
    template<class T>
    struct Component : BaseComponent {
        static const component_id ID;
        static const component_size TYPE_SIZE;
        static const ComponentCreateFunction createFunction;
        static const ComponentDestroyFunction destroyFunction;
    };

    template<class Component>
    u32 createComponent(component_data& data, entity_id entityId, BaseComponent* component) {
        u32 componentIndex = data.size();
        data.resize(componentIndex + Component::TYPE_SIZE);
        // we use new() operator here to just set component_data in memory, rather than allocate it!
        Component* newComponent = new(&data[componentIndex]) Component(*(Component*) component);
        newComponent->entityId = entityId;
        return componentIndex;
    }

    template<class Component>
    void destroyComponent(BaseComponent* component) {
        Component* actualComponent = (Component*) component;
        actualComponent->~Component();
    }

    template<class T>
    const component_id Component<T>::ID(BaseComponent::registerComponentType(
            createComponent<T>, destroyComponent<T>, sizeof(T)
    ));

    template<class T>
    const component_size Component<T>::TYPE_SIZE(sizeof(T));

    template<class T>
    const ComponentCreateFunction Component<T>::createFunction(createComponent<T>);

    template<class T>
    const ComponentDestroyFunction Component<T>::destroyFunction(destroyComponent<T>);

    // System
    class System {

    public:
        explicit System(const vector<component_id>& componentIds) : componentIds(componentIds) {}

    public:
        virtual void onUpdate(Time dt, BaseComponent** components) {}

    public:
        inline const vector<component_id>& getComponentIds() {
            return componentIds;
        }

    private:
        vector<component_id> componentIds;
    };

    typedef vector<std::pair<component_id, u32>> entity_data; // array of [componentId, componentIndex]
    typedef std::pair<u32, entity_data> entity; // entity index -> entity data
    // Registry of Components, Systems, Entities
    class Registry {
        IMMUTABLE(Registry)
    public:
        Registry() = default;
        ~Registry();

    public:
        // entities
        entity_id createEntity();
        template<class Component, typename... Args>
        entity_id createEntity(Args&&... componentArgs);
        void deleteEntity(entity_id entityId);
        // components
        template<class Component, typename... Args>
        void addComponent(entity_id entityId, Args&&... componentArgs);
        template<class Component>
        bool removeComponent(entity_id entityId);
        template<class Component>
        Component* getComponent(entity_id entityId);
        // systems
        void addSystem(System& system);
        bool removeSystem(System& system);
        void updateSystems(Time dt);
        // entity/component iterations
        template<class Component>
        inline void read(const std::function<void(const Component*)>& function) {
            iterate<Component>(function);
        }

        template<class Component>
        inline void write(const std::function<void(Component*)>& function) {
            iterate<Component>(function);
        }

        template<class Component, typename Function>
        void iterate(const std::function<Function>& function);

    private:
        inline entity* toEntity(entity_id entityId) {
            return (entity*) entityId;
        }

        inline u32 toEntityIndex(entity_id entityId) {
            return toEntity(entityId)->first;
        }

        inline entity_data& toEntityData(entity_id entityId) {
            return toEntity(entityId)->second;
        }

    private:
        void removeComponentInternal(component_id componentId, u32 componentIndex);

    private:
        vector<System*> systems;
        map<component_id, component_data> components;
        vector<entity*> entities;
    };

    template<class Component, typename... Args>
    entity_id Registry::createEntity(Args&&... componentArgs) {
        // validate component type
        if (!BaseComponent::isValid<Component>()) {
            ENGINE_ERR("createEntity(): component type with id {0} is invalid!", Component::ID);
            return invalid_entity_id;
        }

        entity_id newEntityId = createEntity();
        addComponent<Component>(newEntityId, std::forward<Args>(componentArgs)...);

        return newEntityId;
    }

    template<class Component, typename... Args>
    void Registry::addComponent(entity_id entityId, Args&&... componentArgs) {
        auto component = Component { std::forward<Args>(componentArgs)... };

        component_id componentId = Component::ID;
        auto createFunction = BaseComponent::getCreateFunction(componentId);
        std::pair<component_id, u32> componentIdAndIndex;
        componentIdAndIndex.first = componentId;
        componentIdAndIndex.second = createFunction(
                components[componentId],
                entityId,
                &component
        );

        entity_data entityData = toEntityData(entityId);
        entityData.emplace_back(componentIdAndIndex);
    }

    template<class Component>
    bool Registry::removeComponent(entity_id entityId) {
        component_id componentId = Component::ID;
        entity_data entityData = toEntityData(entityId);

        for (u32 i = 0 ; i < entityData.size() ; i++) {
            const auto& componentIdAndIndex = entityData[i];

            if (componentId == componentIdAndIndex.first) {
                removeComponentInternal(componentIdAndIndex.first, componentIdAndIndex.second);
                u32 srcIndex = entityData.size() - 1;
                u32 destIndex = i;
                entityData[destIndex] = entityData[srcIndex];
                entityData.pop_back();
                return false;
            }
        }

        return true;
    }

    template<class Component>
    Component* Registry::getComponent(entity_id entityId) {
        entity_data entityData = toEntityData(entityId);
        component_id componentId = Component::ID;

        for (const auto& componentIdAndIndex : entityData) {
            if (componentId == componentIdAndIndex.first) {
                return (Component*) &components[componentId][componentIdAndIndex.second];
            }
        }

        return nullptr;
    }

    template<class Component, typename Function>
    void Registry::iterate(const std::function<Function>& function) {
        component_data& componentData = components[Component::ID];
        component_size componentSize = BaseComponent::getSize(Component::ID);
        for (u32 i = 0 ; i < componentData.size() ; i += componentSize) {
            function((Component*) &componentData[i]);
        }
    }

    class Entity {

    };
}

namespace engine::test {

    using namespace ecs;

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
    }

}