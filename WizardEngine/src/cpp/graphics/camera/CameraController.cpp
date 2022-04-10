//
// Created by mecha on 26.09.2021.
//

#include <graphics/camera/CameraController.h>

namespace engine::graphics {

    CameraController::~CameraController() {
        clearAllBindings();
    }

    void CameraController::bind(const event::KeyCode &keyCode, const MoveType &moveType) {
        moveKeys[keyCode] = moveType;
    }

    void CameraController::bind(const event::KeyCode &keyCode, const RotateType &rotateType) {
        rotateKeys[keyCode] = rotateType;
    }

    void CameraController::bind(const event::KeyCode &keyCode, const ZoomType &zoomType) {
        zoomKeys[keyCode] = zoomType;
    }

    void CameraController::unbindMove(const event::KeyCode &keyCode) {
        moveKeys[keyCode] = NONE_TYPE(MoveType);
    }

    void CameraController::unbindRotate(const event::KeyCode &keyCode) {
        rotateKeys[keyCode] = NONE_TYPE(RotateType);
    }

    void CameraController::unbindZoom(const event::KeyCode &keyCode) {
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

    void CameraController::onKeyPressed(event::KeyCode keyCode) {
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

    void CameraController::onKeyHold(event::KeyCode keyCode) {
        onKeyPressed(keyCode);
    }

    void CameraController::onKeyReleased(event::KeyCode keyCode) {
        // no needs to support it for now!
    }

    void CameraController::onKeyTyped(event::KeyCode keyCode) {
        // no needs to support it for now!
    }

    void CameraController::onWindowClosed() {
    }

    void CameraController::onWindowResized(const uint32_t &width, const uint32_t &height) {
        auto component = getCamera().get<Camera3dComponent>();
        component->viewProjection.perspectiveMatrix.aspectRatio = (float) width / (float) height;
        math::ViewProjections::update(component->viewProjection);
    }

    ecs::Entity Camera2dController::getCamera() {
        return camera;
    }

    ecs::Entity Camera3dController::getCamera() {
        return camera;
    }

    Camera3dController::~Camera3dController() {
    }

    void Camera3dController::zoom(const ZoomType &zoomType) {
        ENGINE_INFO("{0}: zoom()", tag);
        auto &viewPosition = camera.get<Camera3dComponent>()->viewProjection.viewMatrix.position.value;
        switch (zoomType) {
            case ZoomType::ZOOM_IN:
                viewPosition.z -= zoomSpeed / dt;
                break;
            case ZoomType::ZOOM_OUT:
                viewPosition.z += zoomSpeed / dt;
                break;
        }
    }

    void Camera3dController::move(const MoveType &moveType) {
        ENGINE_INFO("{0}: move()", tag);
        auto &viewPosition = camera.get<Camera3dComponent>()->viewProjection.viewMatrix.position.value;
        switch (moveType) {
            case MoveType::DOWN:
                ENGINE_INFO("Camera moved: DOWN");
                viewPosition.y += moveSpeed / dt;
                break;
            case MoveType::UP:
                ENGINE_INFO("Camera moved: UP");
                viewPosition.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT:
                ENGINE_INFO("Camera moved: LEFT");
                viewPosition.x += moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                ENGINE_INFO("Camera moved: RIGHT");
                viewPosition.x -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                ENGINE_INFO("Camera moved: RIGHT_DOWN");
                viewPosition.x -= moveSpeed / dt;
                viewPosition.y += moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                ENGINE_INFO("Camera moved: RIGHT_UP");
                viewPosition.x -= moveSpeed / dt;
                viewPosition.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                ENGINE_INFO("Camera moved: LEFT_DOWN");
                viewPosition.x += moveSpeed / dt;
                viewPosition.y += moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                ENGINE_INFO("Camera moved: LEFT_UP");
                viewPosition.x += moveSpeed / dt;
                viewPosition.y -= moveSpeed / dt;
                break;
        }
    }

    void Camera3dController::rotate(const RotateType &rotateType) {
        ENGINE_INFO("{0}: rotate()", tag);
        auto component = camera.get<Camera3dComponent>();
        switch (rotateType) {
            case RotateType::LEFT_X:
                ENGINE_INFO("Camera rotate: LEFT_X");
                component->viewProjection.viewMatrix.rotation.x -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Y:
                ENGINE_INFO("Camera rotate: LEFT_Y");
                component->viewProjection.viewMatrix.rotation.y -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Z:
                ENGINE_INFO("Camera rotate: LEFT_Z");
                component->viewProjection.viewMatrix.rotation.z -= rotateSpeed / dt;
                break;
            case RotateType::RIGHT_X:
                ENGINE_INFO("Camera rotate: RIGHT_X");
                component->viewProjection.viewMatrix.rotation.x += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Y:
                ENGINE_INFO("Camera rotate: RIGHT_Y");
                component->viewProjection.viewMatrix.rotation.y += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Z:
                ENGINE_INFO("Camera rotate: RIGHT_Z");
                component->viewProjection.viewMatrix.rotation.z += rotateSpeed / dt;
                break;
        }
    }

    void Camera3dController::applyChanges() {
        auto component = camera.get<Camera3dComponent>();
        ENGINE_INFO("{0}: applyChanges()", tag);
        ENGINE_INFO("{0}: position({1},{2},{3})", tag, component->viewProjection.viewMatrix.position.value.x,
                    component->viewProjection.viewMatrix.position.value.y, component->viewProjection.viewMatrix.position.value.z);
        ENGINE_INFO("{0}: rotation({1},{2},{3})", tag, component->viewProjection.viewMatrix.rotation.x, component->viewProjection.viewMatrix.rotation.y,
                    component->viewProjection.viewMatrix.rotation.z);
        ENGINE_INFO("{0}: scale({1})", tag, component->viewProjection.viewMatrix.scale);
        math::ViewProjections::update(component->viewProjection);
    }

    void Camera3dController::setCamera(const Camera2D &camera) {
        // do nothing here!
    }

    void Camera3dController::setCamera(const Camera3D &camera) {
        this->camera = camera;
    }

    Camera2dController::~Camera2dController() {
    }

    void Camera2dController::zoom(const ZoomType &zoomType) {
        ENGINE_INFO("{0}: zoom()", tag);
        auto component = camera.get<Camera2dComponent>();
        switch (zoomType) {
            case ZoomType::ZOOM_IN:
                component->viewProjection.viewMatrix.position.z += zoomSpeed / dt;
                break;
            case ZoomType::ZOOM_OUT:
                component->viewProjection.viewMatrix.position.z -= zoomSpeed / dt;
                break;
        }
    }

    void Camera2dController::move(const MoveType &moveType) {
        ENGINE_INFO("{0}: move()", tag);
        auto component = camera.get<Camera2dComponent>();
        switch (moveType) {
            case MoveType::DOWN:
                component->viewProjection.viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::UP:
                component->viewProjection.viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::LEFT:
                component->viewProjection.viewMatrix.position.x += moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                component->viewProjection.viewMatrix.position.x -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                component->viewProjection.viewMatrix.position.x -= moveSpeed / dt;
                component->viewProjection.viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                component->viewProjection.viewMatrix.position.x -= moveSpeed / dt;
                component->viewProjection.viewMatrix.position.y += moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                component->viewProjection.viewMatrix.position.x += moveSpeed / dt;
                component->viewProjection.viewMatrix.position.y -= moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                component->viewProjection.viewMatrix.position.x += moveSpeed / dt;
                component->viewProjection.viewMatrix.position.y += moveSpeed / dt;
                break;
        }
    }

    void Camera2dController::rotate(const RotateType &rotateType) {
        ENGINE_INFO("{0}: rotate()", tag);
        auto component = camera.get<Camera2dComponent>();

        if (rotateType == RotateType::LEFT_X ||
            rotateType == RotateType::LEFT_Y ||
            rotateType == RotateType::LEFT_Z) {
            component->viewProjection.viewMatrix.rotation -= rotateSpeed;
            return;
        }

        if (rotateType == RotateType::RIGHT_X ||
            rotateType == RotateType::RIGHT_Y ||
            rotateType == RotateType::RIGHT_Z) {
            component->viewProjection.viewMatrix.rotation += rotateSpeed;
            return;
        }
    }

    void Camera2dController::applyChanges() {
        math::ViewProjections::update(camera.get<Camera2dComponent>()->viewProjection);
    }

    void Camera2dController::setCamera(const Camera2D &camera) {
        this->camera = camera;
    }

    void Camera2dController::setCamera(const Camera3D &camera) {
        // do nothing here!
    }

    void Camera2dController::setPosition(const glm::vec3 &position) {
        auto component = camera.get<Camera2dComponent>();
        component->viewProjection.viewMatrix.position = position;
        math::ViewProjections::update(component->viewProjection);
    }

    void Camera3dController::setPosition(const glm::vec3 &position) {
        auto component = camera.get<Camera3dComponent>();
        component->viewProjection.viewMatrix.position.value = position;
        math::ViewProjections::update(component->viewProjection);
    }
}