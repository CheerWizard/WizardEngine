//
// Created by mecha on 03.10.2021.
//

#pragma once

#include <time/Time.h>
#include <core/String.h>
#include <ecs/Scene.h>
#include <graphics/text/Text.h>
#include <platform/tools/FileDialog.h>

namespace engine::visual {

    using namespace ecs;
    using namespace engine::graphics;
    using namespace engine::tools;

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

    struct ENGINE_API CubemapItem final {
        const char* title;
        u32 textureId = invalidTextureId;
        std::string filepath = "";

        CubemapItem(const char* title) : title(title) {}
    };

    struct ENGINE_API CubemapItems final {
        CubemapItem items[6] = { "Front", "Back", "Left", "Right", "Top", "Bottom" };

        CubemapItem& operator [](int i) {
            return items[i];
        }
    };

    struct ENGINE_API HdrEnvItem final {
        u32 textureId = invalidTextureId;
        std::string filepath = "";
    };

    class ENGINE_API SceneHierarchy final {

    public:
        SceneHierarchy(void* nativeWindow, SceneHierarchyCallback* callback);
        ~SceneHierarchy();

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
        Entity _selectedEntity;
        Ref<Scene> selectedScene = nullptr;
        SceneHierarchyCallback* _callback = nullptr;
        bool sceneRenameMode = false;
        bool entityRenameMode = false;

        static Ref<FileDialog> s_FileDialog;
        static unordered_map<entity_id, CubemapItems> s_CubemapItemStorage;
        static unordered_map<entity_id, HdrEnvItem> s_HdrEnvStorage;
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