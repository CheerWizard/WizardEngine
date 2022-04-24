//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Camera.h"

#include <time/Time.h>
#include <core/Memory.h>
#include <event/KeyCodes.h>

#include "unordered_map"

#define DEFAULT_CAMERA_ZOOM_SPEED 0.001f
#define DEFAULT_CAMERA_MOVE_SPEED 0.001f
#define DEFAULT_CAMERA_ROTATE_SPEED 0.001f
#define NONE_TYPE(T) static_cast<T>(0)

namespace engine::graphics {

    enum ZoomType : unsigned char {
        ZOOM_IN = 1, ZOOM_OUT = 2
    };

    enum MoveType : unsigned char {
        LEFT = 1, RIGHT = 2,
        UP = 3, DOWN = 4,
        LEFT_UP = 5, LEFT_DOWN = 6,
        RIGHT_UP = 7, RIGHT_DOWN = 8
    };

    enum RotateType : unsigned char {
        LEFT_X = 1, LEFT_Y = 2, LEFT_Z = 3,
        RIGHT_X = 4, RIGHT_Y = 5, RIGHT_Z = 6
    };

    typedef std::unordered_map<event::KeyCode, ZoomType> ZoomKeys;
    typedef std::unordered_map<event::KeyCode, RotateType> RotateKeys;
    typedef std::unordered_map<event::KeyCode, MoveType> MoveKeys;

    class CameraController {

    public:
        virtual ~CameraController();

    public:
        void zoom(const ZoomType &zoomType, const float &zoomSpeed) {
            this->zoomSpeed = zoomSpeed;
            zoom(zoomType);
        }

        void move(const MoveType &moveType, const float &moveSpeed) {
            this->moveSpeed = moveSpeed;
            move(moveType);
        }

        void rotate(const RotateType &rotateType, const float &rotateSpeed) {
            this->rotateSpeed = rotateSpeed;
            rotate(rotateType);
        }

        void applyMove(const MoveType& moveType);
        void applyRotate(const RotateType& rotateType);
        void applyZoom(const ZoomType& zoomType);

        void updatePosition(const glm::vec3 &position) {
            setPosition(position);
            applyChanges();
        }

        inline void setDeltaTime(const time::Time &dt) {
            this->dt = dt;
        }

    public:
        void bind(const event::KeyCode &keyCode, const MoveType &moveType);
        void bind(const event::KeyCode &keyCode, const RotateType &rotateType);
        void bind(const event::KeyCode &keyCode, const ZoomType &zoomType);

        void unbindMove(const event::KeyCode &keyCode);
        void unbindRotate(const event::KeyCode &keyCode);
        void unbindZoom(const event::KeyCode &keyCode);

        void clearZoomBindings();
        void clearRotateBindings();
        void clearMoveBindings();
        void clearAllBindings();

    public:
        virtual void zoom(const ZoomType &zoomType) = 0;
        virtual void move(const MoveType &moveType) = 0;
        virtual void rotate(const RotateType &rotateType) = 0;
        virtual void applyChanges() = 0;
        virtual void setPosition(const glm::vec3 &position) = 0;
        virtual void setCamera(const Camera2D &camera) = 0;
        virtual void setCamera(const Camera3D &camera) = 0;

        void onKeyPressed(event::KeyCode keyCode);
        void onKeyHold(event::KeyCode keyCode);
        void onKeyReleased(event::KeyCode keyCode);
        void onKeyTyped(event::KeyCode keyCode);

        void onWindowClosed();
        void onWindowResized(const uint32_t &width, const uint32_t &height);

    protected:
        virtual ecs::Entity getCamera() = 0;

    protected:
        MoveKeys moveKeys = MoveKeys();
        RotateKeys rotateKeys = RotateKeys();
        ZoomKeys zoomKeys = ZoomKeys();
        time::Time dt = 1;

    public:
        float zoomSpeed = DEFAULT_CAMERA_ZOOM_SPEED;
        float moveSpeed = DEFAULT_CAMERA_MOVE_SPEED;
        float rotateSpeed = DEFAULT_CAMERA_ROTATE_SPEED;

    };

    class Camera3dController : public CameraController {

    public:
        Camera3dController(const char* tag, const Camera3D &camera) : tag(tag), camera(camera) {}

        ~Camera3dController() override;

    public:
        void zoom(const ZoomType &zoomType) override;
        void move(const MoveType &moveType) override;
        void rotate(const RotateType &rotateType) override;
        void applyChanges() override;

        void setPosition(const glm::vec3 &position) override;

    public:
        void setCamera(const Camera2D &camera) override;
        void setCamera(const Camera3D &camera) override;

    protected:
        ecs::Entity getCamera() override;

    private:
        Camera3D camera;
        const char* tag;

    };

    // todo fix 2D projection!
    class Camera2dController : public CameraController {

    public:
        Camera2dController(const char* tag, const Camera2D &camera) : tag(tag), camera(camera) {}

        ~Camera2dController() override;

        void zoom(const ZoomType &zoomType) override;
        void move(const MoveType &moveType) override;
        void rotate(const RotateType &rotateType) override;
        void applyChanges() override;

        void setPosition(const glm::vec3 &position) override;

    public:
        void setCamera(const Camera2D &camera) override;
        void setCamera(const Camera3D &camera) override;

    protected:
        ecs::Entity getCamera() override;

    private:
        Camera2D camera;
        const char* tag;

    };

}
