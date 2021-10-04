//
// Created by mecha on 03.10.2021.
//

#include "EntityLayout.h"

#include "imgui.h"

namespace engine {

    void EntityLayout::clear() {
        scene->clear();
    }

    Entity EntityLayout::create(const std::string &tag) {
        return scene->createEntity(tag);
    }

    Entity EntityLayout::create() {
        return scene->createEntity();
    }

    void EntityLayout::remove(const Entity &entity) {
        scene->deleteEntity(entity);
    }

    void EntityLayout::destroy() {
        clear();
    }

    void EntityLayout::onUpdate(Time deltaTime) {
        ENGINE_INFO("onUpdate()");
        ImGui::Begin(props.name);
        // todo define list of entities and buttons to manipulate with them.
        ImGui::End();
    }

}