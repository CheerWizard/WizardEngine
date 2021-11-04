//
// Created by mecha on 03.10.2021.
//

#include "SceneHierarchy.h"

#include "../ecs/Components.h"
#include "../graphics/transform/TransformComponents.h"
#include "../graphics/light/LightComponents.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace engine {

    void SceneHierarchy::drawEntityNode(Entity &entity) {
        auto& tag = entity.get<TagComponent>().tag;

        ImGuiTreeNodeFlags headerTreeFlags = ((_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        headerTreeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool headerOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, headerTreeFlags, "%s", tag.c_str());
        if (ImGui::IsItemClicked()) {
            _selectedEntity = entity;
        }

        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Delete Entity")) {
                _scene->deleteEntity(entity);

                if (_selectedEntity == entity) {
                    _selectedEntity = {};
                }

                if (_callback != nullptr) {
                    _callback->onEntityRemoved(entity);
                }
            }

            ImGui::EndPopup();
        }

        if (headerOpened) {
            ImGuiTreeNodeFlags bodyTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool bodyOpened = ImGui::TreeNodeEx((void*)9817239, bodyTreeFlags, "%s", tag.c_str());
            if (bodyOpened) {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

    }

    struct FloatRange {
        float begin;
        float end;
    };

    static void drawFloatController(
            const std::string &label,
            float &value,
            bool &isUpdated,
            const FloatRange &range = {0, 1},
            const float &resetValue = 0.0f
    ) {
        isUpdated = ImGui::SliderFloat(label.c_str(), &value, range.begin, range.end);
    }

    static void drawVec3Controller(
            const std::string& label,
            glm::vec3& values,
            bool &isUpdated,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);

        auto xClicked = ImGui::Button("X", buttonSize);
        if (xClicked) {
            values.x = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto xDragged = ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            values.y = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto yDragged = ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);

        auto zClicked = ImGui::Button("Z", buttonSize);
        if (zClicked) {
            values.z = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto zDragged = ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        isUpdated = xClicked || xDragged || yClicked || yDragged || zClicked || zDragged;
    }

    template<typename T, typename UIFunction>
    static void drawComponent(const std::string& name, const Entity &entity, UIFunction uiFunction) {
        if (!entity.template has<T>()) return;

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
                | ImGuiTreeNodeFlags_Framed
                | ImGuiTreeNodeFlags_SpanAvailWidth
                | ImGuiTreeNodeFlags_AllowItemOverlap
                | ImGuiTreeNodeFlags_FramePadding;

        auto& component = entity.template get<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();

        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());

        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

        if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove component")) {
                removeComponent = true;
            }

            ImGui::EndPopup();
        }

        if (open) {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent) {
            entity.template remove<T>();
        }
    }

    void SceneHierarchy::drawComponents(Entity &entity) {
        auto& tag = entity.get<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy(buffer, tag.c_str(), sizeof(buffer));

        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponent");
        }

        ImGui::PopItemWidth();

        drawComponent<Transform3dComponent>("Transform", entity, [](Transform3dComponent& transform) {
            bool isPosUpdated = false;
            bool isRotUpdated = false;
            bool isScaleUpdated = false;

            drawVec3Controller("Translation", transform.position, isPosUpdated);

            glm::vec3 rotation = glm::degrees(transform.rotation);
            drawVec3Controller("Rotation", rotation, isRotUpdated);
            transform.rotation = glm::radians(rotation);

            drawVec3Controller("Scale", transform.scale, isScaleUpdated, 1.0f);

            transform.isUpdated = isPosUpdated || isRotUpdated || isScaleUpdated;
            if (transform.isUpdated) {
                transform.applyChanges();
            }
            CLIENT_INFO("Transform is updated : {0}", transform.isUpdated);
        });

        drawComponent<AmbientLightComponent>("AmbientLight", entity, [](AmbientLightComponent& ambientLight) {
            drawVec3Controller("Color", ambientLight.color.value, ambientLight.color.isUpdated);
            drawFloatController("Strength", ambientLight.strength.value, ambientLight.strength.isUpdated, {0, 1.5 });
        });

        drawComponent<DiffuseLightComponent>("DiffuseLight", entity, [](DiffuseLightComponent& diffuseLight) {
            drawVec3Controller("Position", diffuseLight.position.value, diffuseLight.position.isUpdated);
            drawVec3Controller("Color", diffuseLight.color.value, diffuseLight.color.isUpdated);
        });

        drawComponent<SpecularLightComponent>("SpecularLight", entity, [](SpecularLightComponent& specularLight) {
            drawVec3Controller("Position", specularLight.position.value, specularLight.position.isUpdated);
            drawVec3Controller("Color", specularLight.color.value, specularLight.color.isUpdated);
            drawFloatController("Strength", specularLight.strength.value, specularLight.strength.isUpdated, { 0, 5 });
        });
    }

    void SceneHierarchy::onUpdate(Time dt) {
        ImGui::Begin(_props.name);

        _scene->getRegistry().each([&](auto entityID) {
            Entity entity { entityID , _scene.get() };
            drawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                _scene->createEntity3d("Empty Entity");
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (_selectedEntity) {
            drawComponents(_selectedEntity);
        }

        ImGui::End();
    }

    void SceneHierarchy::destroy() {
        removeCallback();
    }

}