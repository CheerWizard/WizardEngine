//
// Created by mecha on 26.09.2021.
//

#include "CameraController.h"

namespace engine {

    CameraController::~CameraController() {
        clearAllBindings();
    }

    void CameraController::bind(const KeyCode &keyCode, const MoveType &moveType) {
        moveKeys[keyCode] = moveType;
    }

    void CameraController::bind(const KeyCode &keyCode, const RotateType &rotateType) {
        rotateKeys[keyCode] = rotateType;
    }

    void CameraController::bind(const KeyCode &keyCode, const ZoomType &zoomType) {
        zoomKeys[keyCode] = zoomType;
    }

    void CameraController::unbindMove(const KeyCode &keyCode) {
        moveKeys[keyCode] = UNDEFINED_TYPE(MoveType);
    }

    void CameraController::unbindRotate(const KeyCode &keyCode) {
        rotateKeys[keyCode] = UNDEFINED_TYPE(RotateType);
    }

    void CameraController::unbindZoom(const KeyCode &keyCode) {
        zoomKeys[keyCode] = UNDEFINED_TYPE(ZoomType);
    }

    void CameraController::clearZoomBindings() {
        zoomKeys.clear();
    }

    void CameraController::clearRotateBindings() {
        rotateKeys.clear();
    }

    void CameraController::clearMoveBindings() {
        moveKeys.clear();
    }

    void CameraController::clearAllBindings() {
        clearZoomBindings();
        clearMoveBindings();
        clearRotateBindings();
    }

    void CameraController::onKeyPressed(KeyCode keyCode) {
        auto moveKey = moveKeys[keyCode];
        if (moveKey != UNDEFINED_TYPE(MoveType)) {
            move(moveKey);
            applyChanges();
            return;
        }

        auto zoomKey = zoomKeys[keyCode];
        if (zoomKey != UNDEFINED_TYPE(ZoomType)) {
            zoom(zoomKey);
            applyChanges();
            return;
        }

        auto rotateKey = rotateKeys[keyCode];
        if (rotateKey != UNDEFINED_TYPE(RotateType)) {
            rotate(rotateKey);
            applyChanges();
            return;
        }
    }

    void CameraController::onKeyHold(KeyCode keyCode) {
        onKeyPressed(keyCode);
    }

    void CameraController::onKeyReleased(KeyCode keyCode) {
        // no needs to support it for now!
    }

    void CameraController::onKeyTyped(KeyCode keyCode) {
        // no needs to support it for now!
    }

    Camera3dController::~Camera3dController() {
        delete _camera3D;
    }

    void Camera3dController::zoom(const ZoomType &zoomType) {
        switch (zoomType) {
            case ZoomType::IN:
                _camera3D->viewMatrix.position.z += zoomSpeed;
                _camera3D->viewMatrix.position.x += zoomSpeed;
                _camera3D->viewMatrix.position.y += zoomSpeed;
                break;
            case ZoomType::OUT:
                _camera3D->viewMatrix.position.z -= zoomSpeed;
                _camera3D->viewMatrix.position.x -= zoomSpeed;
                _camera3D->viewMatrix.position.y -= zoomSpeed;
                break;
        }
    }

    void Camera3dController::move(const MoveType &moveType) {
        switch (moveType) {
            case MoveType::DOWN:
                _camera3D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::UP:
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::LEFT:
                _camera3D->viewMatrix.position.x += moveSpeed;
                break;
            case MoveType::RIGHT:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                break;
            case MoveType::RIGHT_DOWN:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                _camera3D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::RIGHT_UP:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::LEFT_DOWN:
                _camera3D->viewMatrix.position.x += moveSpeed;
                _camera3D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::LEFT_UP:
                _camera3D->viewMatrix.position.x += moveSpeed;
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
        }
    }

    void Camera3dController::rotate(const RotateType &rotateType) {
        switch (rotateType) {
            case RotateType::LEFT_X:
                _camera3D->viewMatrix.rotation.x -= rotateSpeed;
                break;
            case RotateType::LEFT_Y:
                _camera3D->viewMatrix.rotation.y -= rotateSpeed;
                break;
            case RotateType::LEFT_Z:
                _camera3D->viewMatrix.rotation.z -= rotateSpeed;
                break;
            case RotateType::RIGHT_X:
                _camera3D->viewMatrix.rotation.x += rotateSpeed;
                break;
            case RotateType::RIGHT_Y:
                _camera3D->viewMatrix.rotation.y += rotateSpeed;
                break;
            case RotateType::RIGHT_Z:
                _camera3D->viewMatrix.rotation.z += rotateSpeed;
                break;
        }
    }

    void Camera3dController::applyChanges() {
        _camera3D->applyChanges();
    }

    Mat4fUniform& Camera3dController::getCamera() {
        return *_camera3D;
    }

    Camera2dController::~Camera2dController() {
        delete _camera2D;
    }

    void Camera2dController::zoom(const ZoomType &zoomType) {
        switch (zoomType) {
            case ZoomType::IN:
                _camera2D->viewMatrix.position.z += zoomSpeed;
                break;
            case ZoomType::OUT:
                _camera2D->viewMatrix.position.z -= zoomSpeed;
                break;
        }
    }

    void Camera2dController::move(const MoveType &moveType) {
        switch (moveType) {
            case MoveType::DOWN:
                _camera2D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::UP:
                _camera2D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::LEFT:
                _camera2D->viewMatrix.position.x += moveSpeed;
                break;
            case MoveType::RIGHT:
                _camera2D->viewMatrix.position.x -= moveSpeed;
                break;
            case MoveType::RIGHT_DOWN:
                _camera2D->viewMatrix.position.x -= moveSpeed;
                _camera2D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::RIGHT_UP:
                _camera2D->viewMatrix.position.x -= moveSpeed;
                _camera2D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::LEFT_DOWN:
                _camera2D->viewMatrix.position.x += moveSpeed;
                _camera2D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::LEFT_UP:
                _camera2D->viewMatrix.position.x += moveSpeed;
                _camera2D->viewMatrix.position.y += moveSpeed;
                break;
        }
    }

    void Camera2dController::rotate(const RotateType &rotateType) {
        if (rotateType == RotateType::LEFT_X ||
        rotateType == RotateType::LEFT_Y ||
        rotateType == RotateType::LEFT_Z) {
            _camera2D->viewMatrix.rotation -= rotateSpeed;
            return;
        }

        if (rotateType == RotateType::RIGHT_X ||
        rotateType == RotateType::RIGHT_Y ||
        rotateType == RotateType::RIGHT_Z) {
            _camera2D->viewMatrix.rotation += rotateSpeed;
            return;
        }
    }

    void Camera2dController::applyChanges() {
        _camera2D->applyChanges();
    }

    Mat4fUniform& Camera2dController::getCamera() {
        return *_camera2D;
    }

    void Camera2dController::setPosition(const glm::vec3 &position) {
        _camera2D->viewMatrix.position = position;
        _camera2D->applyChanges();
    }

    void Camera3dController::setPosition(const glm::vec3 &position) {
        _camera3D->viewMatrix.position = position;
        _camera3D->applyChanges();
    }

}