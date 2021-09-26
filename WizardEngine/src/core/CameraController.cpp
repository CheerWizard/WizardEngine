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
        move(moveKeys[keyCode]);
        rotate(rotateKeys[keyCode]);
        zoom(zoomKeys[keyCode]);
        applyChanges();
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
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::UP:
                _camera3D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::LEFT:
                _camera3D->viewMatrix.position.x += moveSpeed;
                break;
            case MoveType::RIGHT:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                break;
            case MoveType::RIGHT_DOWN:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::RIGHT_UP:
                _camera3D->viewMatrix.position.x -= moveSpeed;
                _camera3D->viewMatrix.position.y -= moveSpeed;
                break;
            case MoveType::LEFT_DOWN:
                _camera3D->viewMatrix.position.x += moveSpeed;
                _camera3D->viewMatrix.position.y += moveSpeed;
                break;
            case MoveType::LEFT_UP:
                _camera3D->viewMatrix.position.x += moveSpeed;
                _camera3D->viewMatrix.position.y -= moveSpeed;
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

}