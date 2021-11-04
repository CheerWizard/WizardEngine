//
// Created by mecha on 03.10.2021.
//

#include "SceneHierarchy.h"

#include "../ecs/Components.h"
#include "../graphics/transform/TransformComponents.h"
#include "../graphics/light/LightComponents.h"
#include "../graphics/material/MaterialComponents.h"

#include <imgui.h>
#include <imgui_internal.h>

#define BUTTON_COLOR ImVec4 { 0.1f, 0.6f, 0.8f, 1 }
#define BUTTON_HOVER_COLOR ImVec4 { 0.2f, 0.7f, 0.9f, 1 }
#define BUTTON_PRESSED_COLOR ImVec4 { 0.1f, 0.6f, 0.8f, 1 }

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

    static void drawFloatSlider(
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

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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

    static void drawVec4Controller(
            const std::string& label,
            glm::vec4& values,
            bool &isUpdated,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
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
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto wClicked = ImGui::Button("W", buttonSize);
        if (wClicked) {
            values.w = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto wDragged = ImGui::DragFloat("##W", &values.w, 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();

        isUpdated = xClicked || xDragged || yClicked || yDragged || zClicked || zDragged || wClicked || wDragged;
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
        // draw tag component
        if (entity.has<TagComponent>()) {
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
        }
        // draw transform component
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
        // draw light components
        drawComponent<AmbientLightComponent>("AmbientLight", entity, [](AmbientLightComponent& ambientLight) {
            drawFloatSlider(ambientLight.strength.name, ambientLight.strength.value, ambientLight.strength.isUpdated, {0, 1.5});
            ambientLight.color.isUpdated = ImGui::ColorPicker3(ambientLight.color.name, ambientLight.color.toFloatPtr());
        });
        drawComponent<DiffuseLightComponent>("DiffuseLight", entity, [](DiffuseLightComponent& diffuseLight) {
            drawVec3Controller(diffuseLight.position.name, diffuseLight.position.value, diffuseLight.position.isUpdated);
            diffuseLight.color.isUpdated = ImGui::ColorPicker3(diffuseLight.color.name, diffuseLight.color.toFloatPtr());
        });
        drawComponent<SpecularLightComponent>("SpecularLight", entity, [](SpecularLightComponent& specularLight) {
            drawVec3Controller(specularLight.position.name, specularLight.position.value, specularLight.position.isUpdated);
            drawFloatSlider(specularLight.strength.name, specularLight.strength.value, specularLight.strength.isUpdated, {0, 5});
            specularLight.color.isUpdated = ImGui::ColorPicker3(specularLight.color.name, specularLight.color.toFloatPtr());
        });
        drawComponent<PhongLightComponent>("PhongLight", entity, [](PhongLightComponent& phongLight) {
            drawVec3Controller(phongLight.position.name, phongLight.position.value, phongLight.position.isUpdated);
            drawFloatSlider(phongLight.ambientStrength.name, phongLight.ambientStrength.value, phongLight.ambientStrength.isUpdated, {0, 1.5});
            drawFloatSlider(phongLight.specularStrength.name, phongLight.specularStrength.value, phongLight.specularStrength.isUpdated, {0, 5});
            phongLight.color.isUpdated = ImGui::ColorPicker3(phongLight.color.name, phongLight.color.toFloatPtr());
        });
        drawComponent<DirectionLightComponent>("DirectionLight", entity, [](DirectionLightComponent& directionLight) {
            drawVec3Controller(directionLight.direction.name, directionLight.direction.value, directionLight.direction.isUpdated);
            drawVec3Controller(directionLight.ambient.name, directionLight.ambient.value, directionLight.ambient.isUpdated);
            drawVec3Controller(directionLight.diffuse.name, directionLight.diffuse.value, directionLight.diffuse.isUpdated);
            drawVec3Controller(directionLight.specular.name, directionLight.specular.value, directionLight.specular.isUpdated);
            directionLight.color.isUpdated = ImGui::ColorPicker3(directionLight.color.name, directionLight.color.toFloatPtr());
        });
        drawComponent<PointLightComponent>("PointLight", entity, [](PointLightComponent& pointLight) {
            drawVec3Controller(pointLight.position.name, pointLight.position.value, pointLight.position.isUpdated);
            drawVec3Controller(pointLight.ambient.name, pointLight.ambient.value, pointLight.ambient.isUpdated);
            drawVec3Controller(pointLight.diffuse.name, pointLight.diffuse.value, pointLight.diffuse.isUpdated);
            drawVec3Controller(pointLight.specular.name, pointLight.specular.value, pointLight.specular.isUpdated);
            drawFloatSlider(pointLight.constant.name, pointLight.constant.value, pointLight.constant.isUpdated, {0, 5});
            drawFloatSlider(pointLight.linear.name, pointLight.linear.value, pointLight.linear.isUpdated, {0, 5});
            drawFloatSlider(pointLight.quadratic.name, pointLight.quadratic.value, pointLight.quadratic.isUpdated, {0, 5});
            pointLight.color.isUpdated = ImGui::ColorPicker3(pointLight.color.name, pointLight.color.toFloatPtr());
        });
        // draw material components
        drawComponent<MaterialComponent>("Material", entity, [](MaterialComponent& material) {
            drawVec3Controller(material.ambient.name, material.ambient.value, material.ambient.isUpdated);
            drawVec3Controller(material.diffuse.name, material.diffuse.value, material.diffuse.isUpdated);
            drawVec3Controller(material.specular.name, material.specular.value, material.specular.isUpdated);
            drawFloatSlider(material.shiny.name, material.shiny.value, material.shiny.isUpdated, { 0, 32 });
            material.color.isUpdated = ImGui::ColorPicker4(material.color.name, material.color.toFloatPtr());
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