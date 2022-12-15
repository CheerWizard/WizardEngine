//
// Created by mecha on 02.04.2022.
//

#pragma once

#include <core/identifier.h>
#include <core/vector.h>
#include <map>
#include <time/Time.h>
#include <core/immutable.h>
#include <tuple>
#include <serialization/serialization.h>

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

    // meta-data of any "component" type
    struct ComponentType {
        ComponentCreateFunction createFunction;
        ComponentDestroyFunction destroyFunction;
        component_size size;
        const char* name;

        ComponentType(
                ComponentCreateFunction createFunction,
                ComponentDestroyFunction destroyFunction,
                component_size size,
                const char* name
        ) : createFunction(createFunction), destroyFunction(destroyFunction), size(size), name(name) {}
    };

    struct ENGINE_API BaseComponent {
        entity_id entityId = invalid_entity_id;

    public:
        static component_id registerComponentType(
                ComponentCreateFunction createFunction,
                ComponentDestroyFunction destroyFunction,
                component_size size,
                const char* typeName
        );

    public:
        inline static const ComponentType& getType(component_id id) {
            return componentTypes->at(id);
        }

        inline static ComponentCreateFunction getCreateFunction(component_id id) {
            return componentTypes->at(id).createFunction;
        }

        inline static ComponentDestroyFunction getDestroyFunction(component_id id) {
            return componentTypes->at(id).destroyFunction;
        }

        inline static component_size getSize(component_id id) {
            return componentTypes->at(id).size;
        }

        inline static const char* getTypeName(component_id id) {
            return componentTypes->at(id).name;
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
#define component(type) struct type : engine::ecs::Component<type>
#define empty_component(type) component(type) {};
#define template_component(component_type, template_type) \
template<typename template_type>                          \
struct component_type : engine::ecs::Component<component_type<template_type>>

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
        ((Component*) component)->~Component();
    }

    template<class T>
    const component_id Component<T>::ID(BaseComponent::registerComponentType(
            createComponent<T>, destroyComponent<T>, sizeof(T), typeid(T).name()
    ));

    template<class T>
    const component_size Component<T>::TYPE_SIZE(sizeof(T));

    template<class T>
    const ComponentCreateFunction Component<T>::createFunction(createComponent<T>);

    template<class T>
    const ComponentDestroyFunction Component<T>::destroyFunction(destroyComponent<T>);

    typedef vector<std::pair<component_id, u32>> entity_data; // array of [componentId, componentIndex]
    typedef std::pair<u32, entity_data> entity; // entity index -> entity data
    typedef void (*EntityFunction)(entity_id);
    // Registry of Components, Systems, Entities
    class ENGINE_API Registry {
        IMMUTABLE(Registry)
    public:
        Registry() = default;
        ~Registry();

    public:
        // entities
        entity_id createEntity();
        template<class Component, typename... Args>
        entity_id createEntity(Args&&... componentArgs);
        void deleteEntity(entity_id& entityId);
        // components
        template<class Component, typename... Args>
        bool addComponent(entity_id entityId, Args&&... componentArgs);
        template<class Component>
        bool removeComponent(entity_id entityId);
        template<class Component>
        Component* getComponent(entity_id entityId);
        // entity/component iterations
        template<typename Function>
        void eachEntity(const Function& function);

        template<class Component, typename Function>
        void each(const Function& function);
        template<class Component1, class Component2, typename Function>
        void each(const Function& function);
        template<class Component1, class Component2, class Component3, typename Function>
        void each(const Function& function);

        template<class Component, typename Function>
        void eachPair(const Function& function);
        template<class Component1, class Component2, typename Function>
        void eachPair(const Function& function);

        size_t entity_count();
        template<class Component>
        size_t component_count();

        bool empty_entity();
        template<class Component>
        bool empty_components();

        void clear();

        template<class ByComponent>
        entity_id findEntity(const std::function<bool(ByComponent*)> &condition);
        template<class ByComponent, class ResultComponent>
        ResultComponent* findComponent(const std::function<bool(ByComponent*)> &condition);

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
        std::unordered_map<component_id, component_data> components;
        vector<entity*> entities;
    };

    template<class Component, typename... Args>
    entity_id Registry::createEntity(Args&&... componentArgs) {
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "BaseComponent::isValid failed -> invalid component id!");

        entity_id newEntityId = createEntity();
        addComponent<Component>(newEntityId, std::forward<Args>(componentArgs)...);
        return newEntityId;
    }

    template<class Component, typename... Args>
    bool Registry::addComponent(entity_id entityId, Args&&... componentArgs) {
        ENGINE_ASSERT(entityId != invalid_entity_id, "addComponent() failed -> invalid entity id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "BaseComponent::isValid failed -> invalid component id!");

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
        return true;
    }

    template<class Component>
    bool Registry::removeComponent(entity_id entityId) {
        ENGINE_ASSERT(entityId != invalid_entity_id, "addComponent() failed -> invalid entity id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "BaseComponent::isValid failed -> invalid component id!");

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
                return true;
            }
        }

        return false;
    }

    template<class Component>
    Component* Registry::getComponent(entity_id entityId) {
        ENGINE_ASSERT(entityId != invalid_entity_id, "getComponent() failed -> invalid entity id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "getComponent failed -> invalid component id!");

        auto& entityData = toEntityData(entityId);
        component_id componentId = Component::ID;

        for (const auto& componentIdAndIndex : entityData) {
            if (componentId == componentIdAndIndex.first) {
                return (Component*) &components[componentId][componentIdAndIndex.second];
            }
        }

        return nullptr;
    }

    template<typename Function>
    void Registry::eachEntity(const Function& function) {
        for (entity* entity : entities) {
            function((entity_id) entity);
        }
    }

    template<class Component, typename Function>
    void Registry::each(const Function& function) {
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "BaseComponent::isValid failed -> invalid component id!");

        component_data& componentData = components[Component::ID];
        for (u32 i = 0 ; i < componentData.size() ; i += Component::TYPE_SIZE) {
            Component* component = (Component*) &componentData[i];
            function(component);
        }
    }

    template<class Component1, class Component2, typename Function>
    void Registry::each(const Function& function) {
        ENGINE_ASSERT(BaseComponent::isValid<Component1>(), "BaseComponent::isValid failed -> invalid Component1 id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component2>(), "BaseComponent::isValid failed -> invalid Component2 id!");

        component_data& componentData1 = components[Component1::ID];
        for (u32 i = 0 ; i < componentData1.size() ; i += Component1::TYPE_SIZE) {
            Component1* component1 = (Component1*) &componentData1[i];
            entity_id entityId1 = component1->entityId;
            Component2* component2 = getComponent<Component2>(entityId1);
            if (component2) {
                function(component1, component2);
            }
        }
    }

    template<class Component1, class Component2, class Component3, typename Function>
    void Registry::each(const Function& function) {
        ENGINE_ASSERT(BaseComponent::isValid<Component1>(), "BaseComponent::isValid failed -> invalid Component1 id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component2>(), "BaseComponent::isValid failed -> invalid Component2 id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component3>(), "BaseComponent::isValid failed -> invalid Component3 id!");

        component_data& componentData1 = components[Component1::ID];
        for (u32 i = 0 ; i < componentData1.size() ; i += Component1::TYPE_SIZE) {
            Component1* component1 = (Component1*) &componentData1[i];
            entity_id entityId1 = component1->entityId;
            Component2* component2 = getComponent<Component2>(entityId1);
            Component3* component3 = getComponent<Component3>(entityId1);
            if (component2 && component3) {
                function(component1, component2, component3);
            }
        }
    }

    template<class Component>
    size_t Registry::component_count() {
        return components[Component::ID].size() / Component::TYPE_SIZE;
    }

    template<class Component>
    bool Registry::empty_components() {
        return component_count<Component>() == 0;
    }

    template<class ByComponent>
    entity_id Registry::findEntity(const std::function<bool(ByComponent*)> &condition) {
        ENGINE_ASSERT(BaseComponent::isValid<ByComponent>(), "BaseComponent::isValid failed -> invalid ByComponent id!");

        component_data& componentData = components[ByComponent::ID];
        for (u32 i = 0 ; i < componentData.size() ; i += ByComponent::TYPE_SIZE) {
            ByComponent* actualComponent = (ByComponent*) &componentData[i];
            if (condition(actualComponent)) {
                return actualComponent->entityId;
            }
        }

        return invalid_entity_id;
    }

    template<class ByComponent, class ResultComponent>
    ResultComponent *Registry::findComponent(const std::function<bool(ByComponent*)> &condition) {
        ENGINE_ASSERT(BaseComponent::isValid<ByComponent>(), "BaseComponent::isValid failed -> invalid ByComponent id!");
        ENGINE_ASSERT(BaseComponent::isValid<ResultComponent>(), "BaseComponent::isValid failed -> invalid ResultComponent id!");

        component_data& componentData = components[ByComponent::ID];
        for (u32 i = 0 ; i < componentData.size() ; i += ByComponent::TYPE_SIZE) {
            ByComponent* actualComponent = (ByComponent*) &componentData[i];
            if (condition(actualComponent)) {
                return getComponent<ResultComponent>(actualComponent->entityId);
            }
        }
    }

    template<class Component, typename Function>
    void Registry::eachPair(const Function &function) {
        ENGINE_ASSERT(BaseComponent::isValid<Component>(), "eachPair failed -> invalid Component id!");

        u32 typeSize = Component::TYPE_SIZE;
        component_data& componentData = components[Component::ID];
        for (u32 i = 0; i < componentData.size(); i += typeSize) {
            for (u32 j = i + typeSize; j < componentData.size(); j += typeSize) {
                Component* component1 = (Component*) &componentData[i];
                Component* component2 = (Component*) &componentData[j];
                function(component1, component2);
            }
        }
    }

    template<class Component1, class Component2, typename Function>
    void Registry::eachPair(const Function &function) {
        ENGINE_ASSERT(BaseComponent::isValid<Component1>(), "eachPair failed -> invalid Component1 id!");
        ENGINE_ASSERT(BaseComponent::isValid<Component2>(), "eachPair failed -> invalid Component2 id!");

        u32 typeSize1 = Component1::TYPE_SIZE;
        u32 typeSize2 = Component2::TYPE_SIZE;
        component_data& componentData1 = components[Component1::ID];
        component_data& componentData2 = components[Component2::ID];
        for (u32 i = 0; i < componentData1.size(); i += typeSize1) {
            for (u32 j = 0; j < componentData2.size(); j += typeSize2) {
                Component1* component1 = (Component1*) &componentData1[i];
                Component2* component2 = (Component2*) &componentData2[j];
                function(component1, component2);
            }
        }
    }
}