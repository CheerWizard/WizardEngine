//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/camera/Camera.h>

namespace engine::ecs {

    enum SceneState {
        EDIT, PLAY, SIMULATE
    };

    class ENGINE_API Scene : public EntityContainer {

    public:
        explicit Scene(const std::string& name = "Untitled");
        ~Scene();

    public:
        Entity findEntity(const uuid& uuid);
        Entity findEntity(const UUIDComponent& uuid);
        Entity findEntity(int uuid);
        void findEntity(int uuid, Entity& entity);

        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

        [[nodiscard]] inline const Entity& getSkybox() const {
            return skybox;
        }

        inline void setCamera(const graphics::Camera3D& camera) {
            this->camera = camera;
        }

        [[nodiscard]] inline graphics::Camera3D& getCamera() {
            return camera;
        }

        inline const std::string& getName() {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

        inline void setHdrEnv(const Entity& hdrEnv) {
            this->hdrEnv = hdrEnv;
        }

        [[nodiscard]] inline const Entity& getHdrEnv() const {
            return hdrEnv;
        }

        bool operator==(const Scene& scene) const {
            return name == scene.name;
        }

        [[nodiscard]] inline const uuid& getId() const {
            return id;
        }

        inline void setId(const uuid& newId) {
            id = newId;
        }

    private:
        uuid id;
        std::string name;
        Entity skybox;
        Entity hdrEnv;
        graphics::Camera3D camera;

        // ------------------ Visual Scene for apps ---------------- //
#ifdef VISUAL
    public:
        [[nodiscard]] inline const SceneState& getState() const {
            return state;
        }

        inline void setState(const SceneState& newState) {
            state = newState;
        }

        bool isPaused = false;

        glm::vec2 viewportSize = { 0.0f, 0.0f };
        glm::vec2 viewportBounds[2];

    private:
        SceneState state = EDIT;
#endif
    };

}