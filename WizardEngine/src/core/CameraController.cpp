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
        moveKeys[keyCode] = NONE_TYPE(MoveType);
    }

    void CameraController::unbindRotate(const KeyCode &keyCode) {
        rotateKeys[keyCode] = NONE_TYPE(RotateType);
    }

    void CameraController::unbindZoom(const KeyCode &keyCode) {
        zoomKeys[keyCode] = NONE_TYPE(ZoomType);
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
        if (moveKey != NONE_TYPE(MoveType)) {
            move(moveKey);
            applyChanges();
            return;
        }

        auto zoomKey = zoomKeys[keyCode];
        if (zoomKey != NONE_TYPE(ZoomType)) {
            zoom(zoomKey);
            applyChanges();
            return;
        }

        auto rotateKey = rotateKeys[keyCode];
        if (rotateKey != NONE_TYPE(RotateType)) {
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

    void CameraController::onWindowClosed() {
    }

    void CameraController::onWindowResized(unsigned int width, unsigned int height) {
    }

    Camera3dController::~Camera3dController() {
        delete _camera3D;
    }

    void Camera3dController::zoom(const ZoomType &zoomType) {
        switch (zoomType) {
            case ZoomType::IN:
                _camera3D->viewMatrix.position.z -= zoomSpeed / dt;
                break;
            case ZoomType::OUT:
                _camera3D->viewMatrix.position.z += zoomSpeed / dt;
                break;
        }
    }

    void Camera3dController::move(const MoveType &moveType) {
        switch (moveType) {
            case MoveType::DOWN:
                _camera3D->viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::UP:
                _camera3D->viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT:
                _camera3D->viewMatrix.position.x += moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                _camera3D->viewMatrix.position.x -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                _camera3D->viewMatrix.position.x -= moveSpeed / dt;
                _camera3D->viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                _camera3D->viewMatrix.position.x -= moveSpeed / dt;
                _camera3D->viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                _camera3D->viewMatrix.position.x += moveSpeed / dt;
                _camera3D->viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                _camera3D->viewMatrix.position.x += moveSpeed / dt;
                _camera3D->viewMatrix.position.y -= moveSpeed / dt;
                break;
        }
    }

    void Camera3dController::rotate(const RotateType &rotateType) {
        switch (rotateType) {
            case RotateType::LEFT_X:
                _camera3D->viewMatrix.rotation.x -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Y:
                _camera3D->viewMatrix.rotation.y -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Z:
                _camera3D->viewMatrix.rotation.z -= rotateSpeed / dt;
                break;
            case RotateType::RIGHT_X:
                _camera3D->viewMatrix.rotation.x += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Y:
                _camera3D->viewMatrix.rotation.y += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Z:
                _camera3D->viewMatrix.rotation.z += rotateSpeed / dt;
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
                _camera2D->viewMatrix.position.z += zoomSpeed / dt;
                break;
            case ZoomType::OUT:
                _camera2D->viewMatrix.position.z -= zoomSpeed / dt;
                break;
        }
    }

    void Camera2dController::move(const MoveType &moveType) {
        switch (moveType) {
            case MoveType::DOWN:
                _camera2D->viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::UP:
                _camera2D->viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::LEFT:
                _camera2D->viewMatrix.position.x += moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                _camera2D->viewMatrix.position.x -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                _camera2D->viewMatrix.position.x -= moveSpeed / dt;
                _camera2D->viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                _camera2D->viewMatrix.position.x -= moveSpeed / dt;
                _camera2D->viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                _camera2D->viewMatrix.position.x += moveSpeed / dt;
                _camera2D->viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                _camera2D->viewMatrix.position.x += moveSpeed / dt;
                _camera2D->viewMatrix.position.y += moveSpeed / dt;
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

    void Camera2dController::onWindowResized(unsigned int width, unsigned int height) {
    }

    void Camera2dController::onWindowClosed() {
    }

    void Camera3dController::onWindowClosed() {
    }

    void Camera3dController::onWindowResized(unsigned int width, unsigned int height) {
        _camera3D->perspectiveMatrix.aspectRatio = (float) width / (float) height;
        _camera3D->applyChanges();
    }

}