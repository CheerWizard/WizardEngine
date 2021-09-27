//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Events.h"

#include "../math/ViewProjectionMatrix.h"

#include "unordered_map"

#define DEFAULT_CAMERA_ZOOM_SPEED 0.05f
#define DEFAULT_CAMERA_MOVE_SPEED 0.05f
#define DEFAULT_CAMERA_ROTATE_SPEED 0.05f
#define UNDEFINED_TYPE(T) static_cast<T>(0)

namespace engine {

    enum ZoomType : unsigned char {
        IN = 1, OUT = 2
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

    typedef ViewProjection2d Camera2d;
    typedef ViewProjection3d Camera3d;

    typedef std::unordered_map<KeyCode, ZoomType> ZoomKeys;
    typedef std::unordered_map<KeyCode, RotateType> RotateKeys;
    typedef std::unordered_map<KeyCode, MoveType> MoveKeys;

    class CameraController : public KeyboardCallback {

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

    public:
        void bind(const KeyCode &keyCode, const MoveType &moveType);
        void bind(const KeyCode &keyCode, const RotateType &rotateType);
        void bind(const KeyCode &keyCode, const ZoomType &zoomType);

        void unbindMove(const KeyCode &keyCode);
        void unbindRotate(const KeyCode &keyCode);
        void unbindZoom(const KeyCode &keyCode);

        void clearZoomBindings();
        void clearRotateBindings();
        void clearMoveBindings();
        void clearAllBindings();

    public:
        virtual void zoom(const ZoomType &zoomType) = 0;
        virtual void move(const MoveType &moveType) = 0;
        virtual void rotate(const RotateType &rotateType) = 0;
        virtual void applyChanges() = 0;
        virtual Mat4fUniform& getCamera() = 0;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

    protected:
        MoveKeys moveKeys = MoveKeys();
        RotateKeys rotateKeys = RotateKeys();
        ZoomKeys zoomKeys = ZoomKeys();

    public:
        float zoomSpeed = DEFAULT_CAMERA_ZOOM_SPEED;
        float moveSpeed = DEFAULT_CAMERA_MOVE_SPEED;
        float rotateSpeed = DEFAULT_CAMERA_ROTATE_SPEED;

    };

    class Camera3dController : public CameraController {

    public:
        Camera3dController(Camera3d* camera3D) : _camera3D(camera3D) {}
        ~Camera3dController() override;

    public:
        void zoom(const ZoomType &zoomType) override;
        void move(const MoveType &moveType) override;
        void rotate(const RotateType &rotateType) override;
        void applyChanges() override;
        Mat4fUniform& getCamera() override;

    private:
        Camera3d* _camera3D;

    };

    // todo fix 2D projection!
    class Camera2dController : public CameraController {

    public:
        Camera2dController(Camera2d* camera2D) : _camera2D(camera2D) {}
        ~Camera2dController() override;

        void zoom(const ZoomType &zoomType) override;
        void move(const MoveType &moveType) override;
        void rotate(const RotateType &rotateType) override;
        void applyChanges() override;
        Mat4fUniform &getCamera() override;

    private:
        Camera2d* _camera2D;

    };

}
