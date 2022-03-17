//
// Created by mecha on 03.10.2021.
//

#include <imgui/SceneHierarchy.h>
#include <ecs/Components.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/light/LightComponents.h>
#include <graphics/material/MaterialComponents.h>
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
                entity.remove();

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

    static void drawFloatSlider(
            FloatUniform &uniform,
            const FloatRange &range = {0, 1},
            const float &resetValue = 0.0f
    ) {
        uniform.isUpdated = ImGui::SliderFloat(uniform.name, &uniform.value, range.begin, range.end);
    }

    static void drawVec3Controller(
            const std::string& label,
            float* vec3,
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
            vec3[0] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto xDragged = ImGui::DragFloat("##X", &vec3[0], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            vec3[1] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto yDragged = ImGui::DragFloat("##Y", &vec3[1], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto zClicked = ImGui::Button("Z", buttonSize);
        if (zClicked) {
            vec3[2] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto zDragged = ImGui::DragFloat("##Z", &vec3[2], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        isUpdated = xClicked || xDragged || yClicked || yDragged || zClicked || zDragged;
    }

    static void drawVec3Controller(
            const std::string& label,
            Vec3fUniform& uniform,
            bool &isUpdated,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, toFloatPtr(uniform), isUpdated, resetValue, columnWidth);
    }

    static void drawVec3Controller(
            const std::string& label,
            Vec4fUniform& uniform,
            bool &isUpdated,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, toFloatPtr(uniform), isUpdated, resetValue, columnWidth);
    }

    static void drawVec3Controller(
            Vec3fUniform &uniform,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(uniform.name, uniform, uniform.isUpdated, resetValue, columnWidth);
    }

    static void drawVec3Controller(
            const std::string& label,
            glm::vec3& value,
            bool& isUpdated,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, glm::value_ptr(value), isUpdated, resetValue, columnWidth);
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

    void drawCheckBox(BoolUniform &uniform) {
        uniform.isUpdated = ImGui::Checkbox(uniform.name, &uniform.value);
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

            if (ImGui::Button("New", { 92, 22 })) {
                ImGui::OpenPopup("NewComponent");
            }

            ImGui::PopItemWidth();
        }
        // draw transform component
        drawComponent<Transform3dComponent>("Transform", entity, [](Transform3dComponent& transform) {
            bool isPosUpdated = false;
            bool isRotUpdated = false;
            bool isScaleUpdated = false;

            drawVec3Controller("Translation", transform.position, isPosUpdated);
            drawVec3Controller("Rotation", transform.rotation, isRotUpdated);
            drawVec3Controller("Scale", transform.scale, isScaleUpdated, 1.0f);

            transform.isUpdated = isPosUpdated || isRotUpdated || isScaleUpdated;
            if (transform.isUpdated) {
                ModelMatrices::update(transform);
            }
            EDITOR_INFO("Transform is updated : {0}", transform.isUpdated);
        });
        // draw light components
        drawComponent<PhongLightComponent>("PhongLight", entity, [](PhongLightComponent& phongLight) {
            drawVec3Controller(phongLight.position.name, phongLight.position, phongLight.position.isUpdated);
            drawVec3Controller(phongLight.ambient.name, phongLight.ambient, phongLight.ambient.isUpdated);
            drawVec3Controller(phongLight.diffuse.name, phongLight.diffuse, phongLight.diffuse.isUpdated);
            drawVec3Controller(phongLight.specular.name, phongLight.specular, phongLight.specular.isUpdated);
        });
        drawComponent<DirectLightComponent>("DirectLight", entity, [](DirectLightComponent& directLight) {
            drawVec3Controller(directLight.direction.name, directLight.direction, directLight.direction.isUpdated);
            drawVec3Controller(directLight.ambient.name, directLight.ambient, directLight.ambient.isUpdated);
            drawVec3Controller(directLight.diffuse.name, directLight.diffuse, directLight.diffuse.isUpdated);
            drawVec3Controller(directLight.specular.name, directLight.specular, directLight.specular.isUpdated);
        });
        drawComponent<PointLightComponent>("PointLight", entity, [](PointLightComponent& pointLight) {
            drawVec3Controller(pointLight.position.name, pointLight.position, pointLight.position.isUpdated);
            drawVec3Controller(pointLight.ambient.name, pointLight.ambient, pointLight.ambient.isUpdated);
            drawVec3Controller(pointLight.diffuse.name, pointLight.diffuse, pointLight.diffuse.isUpdated);
            drawVec3Controller(pointLight.specular.name, pointLight.specular, pointLight.specular.isUpdated);
            drawFloatSlider(pointLight.constant, {0, 1});
            drawFloatSlider(pointLight.linear, {0, 1});
            drawFloatSlider(pointLight.quadratic, {0, 2});
        });
        drawComponent<FlashLightComponent>("FlashLight", entity, [](FlashLightComponent& flashLight) {
            drawVec3Controller(flashLight.position.name, flashLight.position, flashLight.position.isUpdated);
            drawVec3Controller(flashLight.direction.name, flashLight.direction, flashLight.direction.isUpdated);
            drawFloatSlider(flashLight.cutoff, {0, 1});
            drawFloatSlider(flashLight.outerCutoff, {0, 1});
            drawVec3Controller(flashLight.ambient.name, flashLight.ambient, flashLight.ambient.isUpdated);
            drawVec3Controller(flashLight.diffuse.name, flashLight.diffuse, flashLight.diffuse.isUpdated);
            drawVec3Controller(flashLight.specular.name, flashLight.specular, flashLight.specular.isUpdated);
            drawFloatSlider(flashLight.constant, {0, 1});
            drawFloatSlider(flashLight.linear, {0, 1});
            drawFloatSlider(flashLight.quadratic, {0, 2});
        });
        // draw material components
        drawComponent<MaterialComponent>("Material", entity, [](MaterialComponent& material) {
            drawFloatSlider(material.ambient);
            drawFloatSlider(material.diffuse);
            drawCheckBox(material.diffuseMapEnabled);
            drawFloatSlider(material.specular);
            drawCheckBox(material.specularMapEnabled);
            drawFloatSlider(material.shiny, { 0, 32 });
            material.color.isUpdated = ImGui::ColorPicker4(material.color.name, toFloatPtr(material.color));
        });
    }

    void SceneHierarchy::onUpdate(Time dt) {
        ImGui::Begin(_props.name);

        draw(_scene->getBatchRegistry(), false);
        draw(_scene->getInstanceRegistry(), true);

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                Entity {_scene.get() };
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

    void SceneHierarchy::draw(entt::registry& registry, bool instancingEnabled) {
        if (registry.empty<>()) return;

        registry.each([&](auto entityID) {
            Entity entity { entityID , _scene.get(), instancingEnabled };
            drawEntityNode(entity);
        });
    }

    void SceneHierarchy::destroy() {
        removeCallback();
    }
}