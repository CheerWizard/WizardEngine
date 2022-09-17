//
// Created by mecha on 03.10.2021.
//

#pragma once

#include <time/Time.h>
#include <core/String.h>
#include <ecs/Scene.h>
#include <graphics/text/Text.h>

namespace engine::gui {

    struct ENGINE_API SceneHierarchyProps {
        const char* name = "Scene Hierarchy";
    };

    class ENGINE_API SceneHierarchyCallback {
    public:
        virtual ~SceneHierarchyCallback() = default;

    public:
        virtual void onEntityRemoved(const ecs::Entity &entity) = 0;
    };

    class ENGINE_API SceneHierarchy {

    public:
        SceneHierarchy(const SceneHierarchyProps &props = SceneHierarchyProps()) : _props(props) {}
        SceneHierarchy(const core::Ref<ecs::Scene>& scene, const SceneHierarchyProps &props = SceneHierarchyProps()) :
        _props(props), _scene(scene) {}

        ~SceneHierarchy() {
            destroy();
        }

    public:
        void onUpdate(time::Time dt);

    public:
        inline void setScene(const core::Ref<ecs::Scene> &scene) {
            _scene = scene;
        }

        inline const ecs::Entity& getSelectedEntity() const {
            return _selectedEntity;
        }

        inline void setSelectedEntity(const ecs::Entity& selectedEntity) {
            _selectedEntity = selectedEntity;
        }

        inline void setCallback(SceneHierarchyCallback* callback) {
            _callback = callback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

    private:
        void draw(ecs::Registry& registry);
        void drawEntityNode(ecs::Entity &entity);
        static void drawComponents(ecs::Entity &entity);
        template<typename T>
        static void drawTextComponent(graphics::TextComponent<T>& textComponent);

    private:
        void destroy();

    private:
        SceneHierarchyProps _props;
        core::Ref<ecs::Scene> _scene;
        ecs::Entity _selectedEntity;
        SceneHierarchyCallback* _callback = nullptr;

    };

    template<typename T>
    void SceneHierarchy::drawTextComponent(graphics::TextComponent<T>& textComponent) {
        drawTransform3dController(textComponent.transform);
        drawTextField("##Text", textComponent.text, textComponent.text);
        drawFloatSlider("Padding X", textComponent.paddingX, textComponent.paddingX);
        drawFloatSlider("Padding Y", textComponent.paddingY, textComponent.paddingY);
        drawFloatSlider("WhitespaceWidth", textComponent.whiteSpaceWidth, textComponent.whiteSpaceWidth);
        drawFloatSlider(textComponent.transparency);
        drawColorPicker(textComponent.color);
    }

}