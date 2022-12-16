//
// Created by mecha on 03.10.2021.
//

#pragma once

#include <time/Time.h>
#include <core/String.h>
#include <ecs/Scene.h>
#include <graphics/text/Text.h>

namespace engine::visual {

    using namespace ecs;

    class ENGINE_API SceneHierarchyCallback {
    public:
        virtual ~SceneHierarchyCallback() = default;
    public:
        virtual void onSceneRemoved(const Ref<Scene>& scene) = 0;
        virtual void onEntityRemoved(const Entity& entity) = 0;
        virtual void onSceneSelected(const Ref<Scene>& scene) = 0;
        virtual void onEntitySelected(const Entity& entity) = 0;
    };

    struct ENGINE_API UIPolygonMode final {
        const char* modes[3] { "Fill", "Points", "Lines" };
        const char* faceTypes[3] { "Back", "Front", "Front/Back" };
        int modeIndex = 0;
        int faceTypeIndex = 0;
    };

    struct ENGINE_API UICulling final {
        const char* faceTypes[3] { "Back", "Front", "Front/Back" };
        const char* frontFaceTypes[2] { "Clock Wise", "Counter-Clock Wise" };
        int faceTypeIndex = 0;
        int frontFaceTypeIndex = 0;
    };

    class ENGINE_API SceneHierarchy final {

    public:
        SceneHierarchy(SceneHierarchyCallback* callback) : _callback(callback) {}
        ~SceneHierarchy() {
            destroy();
        }

    public:
        void onUpdate(time::Time dt);

    public:
        inline const ecs::Entity& getSelectedEntity() const {
            return _selectedEntity;
        }

        inline void setSelectedEntity(const Entity& selectedEntity) {
            _selectedEntity = selectedEntity;
        }

        inline void setCallback(SceneHierarchyCallback* callback) {
            _callback = callback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

    private:
        void drawScene(const Ref<Scene>& scene);
        static void drawComponents(Entity &entity);
        template<typename T>
        static void drawTextComponent(graphics::TextComponent<T>& textComponent);

    private:
        void destroy();

    private:
        Entity _selectedEntity;
        Ref<Scene> selectedScene = nullptr;
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