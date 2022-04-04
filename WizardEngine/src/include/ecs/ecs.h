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
    typedef vector<u8> component_data; // components raw storage in bytes

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
            return (*componentTypes)[id];
        }

        inline static ComponentCreateFunction getCreateFunction(component_id id) {
            return std::get<0>((*componentTypes)[id]);
        }

        inline static ComponentDestroyFunction getDestroyFunction(component_id id) {
            return std::get<1>((*componentTypes)[id]);
        }

        inline static component_size getSize(component_id id) {
            return std::get<2>((*componentTypes)[id]);
        }

        inline static bool isValid(component_id id) {
            return componentTypes->size() > id;
        }

        template<class Component>
        inline static bool isValid() {
            return isValid(Component::ID);
        }

    private:
        static vector<ComponentType>* componentTypes;
    };

#define validate_entity(tag, entityId, result) if ((entityId) == invalid_entity_id) { \
        ENGINE_WARN("{0}: entity is invalid!", tag);                 \
        return result;                                                             \
}

#define validate_component(tag, Component, result) if (!BaseComponent::isValid<Component>()) {   \
        ENGINE_WARN("{0}: component type with id {1} is invalid!", tag, Component::ID); \
        return result;                                                            \
}

    // Component

    // used to statically register Component type for further runtime usage
    template<class T>
    struct Component : BaseComponent {
        static const component_id ID;
        static const component_size TYPE_SIZE;
        static const ComponentCreateFunction createFunction;
        static const ComponentDestroyFunction destroyFunction;
    };

    /** You can do some mistakes like:
    struct B : Component<B> { float x, y; }; - correct
    struct A : Component<B> { bool flag; }; - incorrect type registration, which can lead to unexpected bugs.
    And the compiler and runtime will not catch this issue!

    This macro prevents you from doing such mistakes. */
#define component(type) struct type : Component<type>
#define component_empty(type) component(type) {};

    template<class Component>
    u32 createComponent(component_data& data, entity_id entityId, BaseComponent* component) {
        u32 componentIndex = data.size();
        data.resize(componentIndex + Component::TYPE_SIZE);
        // we use new() operator here to just set component_data in memory, rather than allocate it!
        auto* newComponent = new(&data[componentIndex]) Component(*(Component*) component);
        newComponent->entityId = entityId;
        return componentIndex;
    }

    template<class Component>
    void destroyComponent(BaseComponent* component) {
        auto* actualComponent = (Component*) component;
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
        entity_id deleteEntity(entity_id entityId);
        // components
        template<class Component, typename... Args>
        void addComponent(entity_id entityId, Args&&... componentArgs);
        template<class Component>
        bool removeComponent(entity_id entityId);
        template<class Component>
        Component* getComponent(entity_id entityId);
        // entity/component iterations
        template<class Component, typename Function>
        void each(Function function);
        template<class Component1, class Component2, typename Function>
        void each(Function function);

        template<class Component>
        size_t component_count();
        size_t entity_count();

    private:
        static inline entity* toEntity(entity_id entityId) {
            return (entity*) entityId;
        }

        static inline u32 toEntityIndex(entity_id entityId) {
            return toEntity(entityId)->first;
        }

        static inline entity_data& toEntityData(entity_id entityId) {
            return toEntity(entityId)->second;
        }

    private:
        void removeComponentInternal(component_id componentId, u32 componentIndex);

    private:
        map<component_id, component_data> components;
        vector<entity*> entities;
    };

    template<class Component, typename... Args>
    entity_id Registry::createEntity(Args&&... componentArgs) {
        validate_component("createEntity()", Component, invalid_entity_id);

        entity_id newEntityId = createEntity();
        addComponent<Component>(newEntityId, std::forward<Args>(componentArgs)...);
        return newEntityId;
    }

    template<class Component, typename... Args>
    void Registry::addComponent(entity_id entityId, Args&&... componentArgs) {
        validate_entity("addComponent()", entityId, );
        validate_component("addComponent", Component, );

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

        auto& entityData = toEntityData(entityId);
        entityData.emplace_back(componentIdAndIndex);
    }

    template<class Component>
    bool Registry::removeComponent(entity_id entityId) {
        validate_entity("removeComponent()", entityId, false);
        validate_component("removeComponent()", Component, false);

        component_id componentId = Component::ID;
        auto& entityData = toEntityData(entityId);

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
        validate_entity("getComponent()", entityId, invalid_entity_id);
        validate_component("getComponent()", Component, nullptr);

        auto& entityData = toEntityData(entityId);
        component_id componentId = Component::ID;

        for (const auto& componentIdAndIndex : entityData) {
            if (componentId == componentIdAndIndex.first) {
                return (Component*) &components[componentId][componentIdAndIndex.second];
            }
        }

        return nullptr;
    }

    template<class Component, typename Function>
    void Registry::each(Function function) {
        validate_component("iterate()", Component, );

        component_data& componentData = components[Component::ID];
        component_size componentSize = BaseComponent::getSize(Component::ID);
        for (u32 i = 0 ; i < componentData.size() ; i += componentSize) {
            function((Component*) &componentData[i]);
        }
    }

    template<class Component>
    size_t Registry::component_count() {
        return components[Component::ID].size() / BaseComponent::getSize(Component::ID);
    }
}