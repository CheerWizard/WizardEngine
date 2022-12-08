//
// Created by mecha on 03.10.2021.
//

#include <visual/SceneHierarchy.h>

#include <ecs/Components.h>

#include <graphics/transform/TransformComponents.h>
#include <graphics/light/LightComponents.h>
#include <graphics/outline/Outline.h>
#include <graphics/text/Text.h>
#include <visual/MaterialPanel.h>

#include <physics/Physics.h>

#include <imgui.h>
#include <imgui_internal.h>

#define BUTTON_COLOR ImVec4 { 0.1f, 0.6f, 0.8f, 1 }
#define BUTTON_HOVER_COLOR ImVec4 { 0.2f, 0.7f, 0.9f, 1 }
#define BUTTON_PRESSED_COLOR ImVec4 { 0.1f, 0.6f, 0.8f, 1 }

namespace engine::visual {

    using namespace math;
    using namespace physics;

    void SceneHierarchy::drawEntityNode(ecs::Entity &entity) {
        auto& tag = entity.get<ecs::TagComponent>()->tag;

        ImGuiTreeNodeFlags headerTreeFlags = ((_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        headerTreeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool headerOpened = ImGui::TreeNodeEx(entity.getId(), headerTreeFlags, "%s", tag.c_str());
        if (ImGui::IsItemClicked()) {
            _selectedEntity = entity;
        }

        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Delete Entity")) {
                entity.destroy();

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

    static bool drawFloatSlider(
            const char* label,
            float oldValue,
            float& value,
            const FloatRange& range = { 0, 1 }
    ) {
        ImGui::SliderFloat(label, &value, range.begin, range.end);
        return oldValue != value;
    }

    static void drawFloatSlider(
            const std::string &label,
            float &value,
            const FloatRange &range = {0, 1},
            const float &resetValue = 0.0f
    ) {
        ImGui::SliderFloat(label.c_str(), &value, range.begin, range.end);
    }

    static void drawFloatSlider(
            FloatUniform &uniform,
            const FloatRange &range = {0, 1},
            const float &resetValue = 0.0f
    ) {
        ImGui::SliderFloat(uniform.name, &uniform.value, range.begin, range.end);
    }

    static void drawVec3Controller(
            const std::string& label,
            float* vec3,
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
        ImGui::DragFloat("##X", &vec3[0], 0.05f, 0.0f, 0.0f, "%.2f");
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
        ImGui::DragFloat("##Y", &vec3[1], 0.05f, 0.0f, 0.0f, "%.2f");
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
        ImGui::DragFloat("##Z", &vec3[2], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    static void drawVec3Controller(
            const std::string& label,
            Vec3fUniform& uniform,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, toFloatPtr(uniform), resetValue, columnWidth);
    }

    static void drawVec3Controller(
            const std::string& label,
            Vec4fUniform& uniform,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, toFloatPtr(uniform), resetValue, columnWidth);
    }

    static void drawVec3Controller(
            Vec3fUniform &uniform,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(uniform.name, uniform, resetValue, columnWidth);
    }

    static void drawVec3Controller(
            const std::string& label,
            vec3f& value,
            float resetValue = 0.0f,
            float columnWidth = 100.0f
    ) {
        drawVec3Controller(label, math::values(value), resetValue, columnWidth);
    }

    static void drawVec4Controller(
            const std::string& label,
            vec4f& values,
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
            values.v[0] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.v[0], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            values.v[1] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.v[1], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto zClicked = ImGui::Button("Z", buttonSize);
        if (zClicked) {
            values.v[2] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.v[2], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVER_COLOR);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_PRESSED_COLOR);
        ImGui::PushFont(boldFont);

        auto wClicked = ImGui::Button("W", buttonSize);
        if (wClicked) {
            values.v[3] = resetValue;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        auto wDragged = ImGui::DragFloat("##W", &values.v[3], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    template<typename T, typename UIFunction>
    static void drawComponent(const std::string& name, const ecs::Entity &entity, UIFunction uiFunction) {
        if (!entity.template has<T>()) return;

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
                | ImGuiTreeNodeFlags_Framed
                | ImGuiTreeNodeFlags_SpanAvailWidth
                | ImGuiTreeNodeFlags_AllowItemOverlap
                | ImGuiTreeNodeFlags_FramePadding;

        auto& component = *entity.template get<T>();
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

    void drawCheckBox(const char* label, bool& value) {
        ImGui::Checkbox(label, &value);
    }

    void drawCheckBox(BoolUniform &uniform) {
        ImGui::Checkbox(uniform.name, &uniform.value);
    }

    void drawTransformComponent(const ecs::Entity& entity) {
        drawComponent<Transform3dComponent>("Transform", entity, [](graphics::Transform3dComponent& transform) {
            auto& model = transform.modelMatrix;
            drawVec3Controller("Translation", model.position);
            drawVec3Controller("Rotation", model.rotation);
            drawVec3Controller("Scale", model.scale, 1.0f);
            model.apply();
        });
    }

    bool drawTextField(const char* label, std::string &oldText, std::string &text) {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy(buffer, text.c_str(), sizeof(buffer));

        if (ImGui::InputText(label, buffer, sizeof(buffer))) {
            text = std::string(buffer);
        }

        return oldText != text;
    }

    void drawColorPicker(Vec4fUniform& color) {
        ImGui::ColorPicker4(color.name, toFloatPtr(color));
    }

    void drawTransform3dController(graphics::Transform3dComponent& transform) {
        auto& model = transform.modelMatrix;
        drawVec3Controller("Translation", model.position);
        drawVec3Controller("Rotation", model.rotation);
        drawVec3Controller("Scale", model.scale, 1.0f);
        model.apply();
    }

    static unordered_map<entity_id, UIPolygonMode> uiPolygonModes;
    static unordered_map<entity_id, UICulling> uiCullings;

    void SceneHierarchy::drawComponents(ecs::Entity &entity) {
        // draw tag component
        if (entity.has<ecs::TagComponent>()) {
            auto& tag = entity.get<ecs::TagComponent>()->tag;
            drawTextField("##Tag", tag, tag);

            ImGui::SameLine();
            ImGui::PushItemWidth(-1);

            if (ImGui::Button("New", { 92, 22 })) {
                ImGui::OpenPopup("NewComponent");
            }

            ImGui::PopItemWidth();
        }
        // draw transform
        drawTransformComponent(entity);
        // draw light components
        drawComponent<LightComponent>("Light", entity, [](LightComponent& light) {
            ImGui::ColorPicker4(light.color.name, toFloatPtr(light.color));
            drawVec3Controller(light.position.name, light.position);
        });
        drawComponent<PhongLightComponent>("Phong Light", entity, [](graphics::PhongLightComponent& phongLight) {
            drawVec3Controller(phongLight.position.name, phongLight.position);
            drawVec4Controller(phongLight.color.name, phongLight.color.value);
            drawFloatSlider(phongLight.ambient);
            drawFloatSlider(phongLight.diffuse);
            drawFloatSlider(phongLight.specular);
        });
        drawComponent<DirectLightComponent>("Direct Light", entity, [](graphics::DirectLightComponent& directLight) {
            drawVec3Controller(directLight.direction.name, directLight.direction);
            drawVec3Controller(directLight.ambient.name, directLight.ambient);
            drawVec3Controller(directLight.diffuse.name, directLight.diffuse);
            drawVec3Controller(directLight.specular.name, directLight.specular);
        });
        drawComponent<PointLightComponent>("Point Light", entity, [](graphics::PointLightComponent& pointLight) {
            drawVec3Controller(pointLight.position.name, pointLight.position);
            drawVec3Controller(pointLight.ambient.name, pointLight.ambient);
            drawVec3Controller(pointLight.diffuse.name, pointLight.diffuse);
            drawVec3Controller(pointLight.specular.name, pointLight.specular);
            drawFloatSlider(pointLight.constant, {0, 1});
            drawFloatSlider(pointLight.linear, {0, 1});
            drawFloatSlider(pointLight.quadratic, {0, 2});
        });
        drawComponent<FlashLightComponent>("Flash Light", entity, [](graphics::FlashLightComponent& flashLight) {
            drawVec3Controller(flashLight.position.name, flashLight.position);
            drawVec3Controller(flashLight.direction.name, flashLight.direction);
            drawFloatSlider(flashLight.cutoff, {0, 1});
            drawFloatSlider(flashLight.outerCutoff, {0, 1});
            drawVec3Controller(flashLight.ambient.name, flashLight.ambient);
            drawVec3Controller(flashLight.diffuse.name, flashLight.diffuse);
            drawVec3Controller(flashLight.specular.name, flashLight.specular);
            drawFloatSlider(flashLight.constant, {0, 1});
            drawFloatSlider(flashLight.linear, {0, 1});
            drawFloatSlider(flashLight.quadratic, {0, 2});
        });
        // draw outline component
        drawComponent<OutlineComponent>("Outlining", entity, [](graphics::OutlineComponent& outline) {
            drawFloatSlider(outline.thickness, { 0, 0.1 });
            ImGui::ColorPicker4(outline.color.name, toFloatPtr(outline.color));
        });
        // draw text components
        drawComponent<Text2d>("Text2D", entity, [](graphics::Text2d& textComponent) {
            drawTextComponent(textComponent);
        });
        drawComponent<Text3d>("Text3D", entity, [](graphics::Text3d& textComponent) {
            drawTextComponent(textComponent);
        });
        // material
        drawComponent<Material>("Material", entity, [](Material& material) {
            MaterialPanel::drawMaterial(material);
        });
        // polygon mode
        drawComponent<PolygonModeComponent>("PolygonMode", entity, [&entity](PolygonModeComponent& polygonModeComponent) {
            entity_id entityId = entity.getId();
            if (uiPolygonModes.find(entityId) == uiPolygonModes.end()) {
                uiPolygonModes.insert(std::pair<entity_id, UIPolygonMode>(entityId, UIPolygonMode()));
            }
            UIPolygonMode& uiPolygonMode = uiPolygonModes[entityId];
            // polygon modes
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
            ImGui::PushItemWidth(128);
            ImGui::Combo("##polygon_modes", &uiPolygonMode.modeIndex, uiPolygonMode.modes,
                         IM_ARRAYSIZE(uiPolygonMode.modes));
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
            ImGui::SameLine();
            // face types
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
            ImGui::PushItemWidth(128);
            ImGui::Combo("##polygon_face_type", &uiPolygonMode.faceTypeIndex, uiPolygonMode.faceTypes,
                         IM_ARRAYSIZE(uiPolygonMode.faceTypes));
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
            ImGui::SameLine();
        });
        // culling
        drawComponent<CullingComponent>("Culling", entity, [&entity](CullingComponent& cullingComponent) {
            entity_id entityId = entity.getId();
            if (uiCullings.find(entityId) == uiCullings.end()) {
                uiCullings.insert(std::pair<entity_id, UICulling>(entityId, UICulling()));
            }
            UICulling& uiCulling = uiCullings[entityId];
            // face types
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
            ImGui::PushItemWidth(128);
            ImGui::Combo("##culling_face_type", &uiCulling.faceTypeIndex, uiCulling.faceTypes,
                         IM_ARRAYSIZE(uiCulling.faceTypes));
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
            ImGui::SameLine();
            // front face type
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
            ImGui::PushItemWidth(128);
            ImGui::Combo("##culling_front_face_type", &uiCulling.frontFaceTypeIndex, uiCulling.frontFaceTypes,
                         IM_ARRAYSIZE(uiCulling.frontFaceTypes));
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
            ImGui::SameLine();
        });
        // camera
        drawComponent<Camera3dComponent>("Camera3D", entity, [](Camera3dComponent& camera3DComponent) {
            drawVec3Controller("position", camera3DComponent.viewProjection.viewMatrix.position, 1);
            drawFloatSlider("pitch", camera3DComponent.viewProjection.viewMatrix.pitch);
            drawFloatSlider("roll", camera3DComponent.viewProjection.viewMatrix.roll);
            drawFloatSlider("yaw", camera3DComponent.viewProjection.viewMatrix.yaw);
            drawFloatSlider("scale", camera3DComponent.viewProjection.viewMatrix.scale, { 1, 10 }, 1);
            drawFloatSlider("zNear", camera3DComponent.viewProjection.perspectiveMatrix.zNear, { 0, 1 }, DEFAULT_Z_NEAR);
            drawFloatSlider("zFar", camera3DComponent.viewProjection.perspectiveMatrix.zFar, { 0, 5000 }, DEFAULT_Z_FAR);
            drawFloatSlider("FOV", camera3DComponent.viewProjection.perspectiveMatrix.fieldOfView, { 0, 360 }, DEFAULT_FIELD_OF_VIEW);
        });
        // velocity
        drawComponent<Velocity>("Velocity", entity, [](Velocity& velocity) {
            drawVec3Controller("velocity", velocity.velocity);
            drawCheckBox("flipped", velocity.flipped);
        });
        // collision transform
        drawComponent<CollisionTransform>("Collision Transform", entity, [](CollisionTransform& collisionTransform) {
            auto& model = collisionTransform.model;
            drawVec3Controller("translation", model.position);
            drawVec3Controller("rotation", model.rotation);
            drawVec3Controller("scale", model.scale, 1.0f);
            model.apply();
            drawCheckBox("bind to origin", collisionTransform.bindOrigin);
        });
        // sphere collider
        drawComponent<SphereCollider>("Sphere Collider", entity, [](SphereCollider& sphereCollider) {
            drawVec3Controller("center", sphereCollider.center);
            drawFloatSlider("radius", sphereCollider.radius);
        });
        // AABB collider
        drawComponent<AABBCollider>("AABB Collider", entity, [](AABBCollider& aabbCollider) {
            drawVec3Controller("min extents", aabbCollider.minExtents);
            drawVec3Controller("max extents", aabbCollider.maxExtents);
        });
        // Plane collider
        drawComponent<PlaneCollider>("Plane Collider", entity, [](PlaneCollider& planeCollider) {
            drawVec3Controller("normal", planeCollider.normal);
            drawFloatSlider("distance", planeCollider.distance);
            drawVec3Controller("min value", planeCollider.minV);
            drawVec3Controller("max value", planeCollider.maxV);
        });
    }

    void SceneHierarchy::onUpdate(time::Time dt) {
        ImGui::Begin(_props.name);

        draw(_scene->getRegistry());

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                ecs::Entity {_scene.get() };
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

    void SceneHierarchy::draw(ecs::Registry& registry) {
        if (registry.empty_entity()) return;

        registry.eachEntity([&](ecs::entity_id entityID) {
            ecs::Entity entity { _scene.get(), entityID };
            drawEntityNode(entity);
        });
    }

    void SceneHierarchy::destroy() {
        removeCallback();
    }
}