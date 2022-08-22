//
// Created by mecha on 29.10.2021.
//

#include <graphics/camera/Camera.h>
#include <platform/core/Input.h>

namespace engine::graphics {

    void Camera2D::create() {
        add<Camera2dComponent>(Camera2dComponent());
    }

    OrthographicMatrix& Camera2D::getOrtho() {
        return get<Camera2dComponent>()->viewProjection.orthographicMatrix;
    }

    void Camera2D::applyOrtho() {
        math::update(getOrtho());
    }

    ViewMatrix2d &Camera2D::getView() {
        return get<Camera2dComponent>()->viewProjection.viewMatrix;
    }

    void Camera2D::applyView() {
        math::update(getView());
    }

    void Camera2D::onUpdate(time::Time dt) {
        this->dt = dt;
    }

    void Camera2D::zoom(const ZoomType &zoomType) {
        zoom(zoomType, zoomSpeed);
    }

    void Camera2D::zoom(const ZoomType &zoomType, float zoomSpeed) {
        ENGINE_INFO("{0}: zoom()", tag);
        auto& position = getView().position;
        switch (zoomType) {
            case ZoomType::ZOOM_IN:
                position[2] += zoomSpeed / dt;
                break;
            case ZoomType::ZOOM_OUT:
                position[2] -= zoomSpeed / dt;
                break;
        }
    }

    void Camera2D::setPosition(const vec2f &position) {
        getView().position = { position, 0 };
        applyView();
    }

    void Camera2D::move(const MoveType &moveType) {
        move(moveType, moveSpeed);
    }

    void Camera2D::move(const MoveType &moveType, float moveSpeed) {
        ENGINE_INFO("{0}: move()", tag);
        auto& position = getView().position;
        switch (moveType) {
            case MoveType::DOWN:
                position[1] -= moveSpeed / dt;
                break;
            case MoveType::UP:
                position[1] += moveSpeed / dt;
                break;
            case MoveType::LEFT:
                position[0] += moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                position[0] -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                position[0] -= moveSpeed / dt;
                position[1] -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                position[0] -= moveSpeed / dt;
                position[1] += moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                position[0] += moveSpeed / dt;
                position[1] -= moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                position[0] += moveSpeed / dt;
                position[1] += moveSpeed / dt;
                break;
        }
    }

    void Camera2D::rotate(const RotateType &rotateType) {
        rotate(rotateType, rotateSpeed);
    }

    void Camera2D::rotate(const RotateType &rotateType, float rotateSpeed) {
        ENGINE_INFO("{0}: rotate()", tag);
        auto& rotation = getView().rotation;

        if (rotateType == RotateType::LEFT_X ||
            rotateType == RotateType::LEFT_Y ||
            rotateType == RotateType::LEFT_Z) {
            rotation -= rotateSpeed;
            return;
        }

        if (rotateType == RotateType::RIGHT_X ||
            rotateType == RotateType::RIGHT_Y ||
            rotateType == RotateType::RIGHT_Z) {
            rotation += rotateSpeed;
            return;
        }
    }

    void Camera2D::move(const vec2f &position) {
        auto& viewPosition = getView().position;
        viewPosition[0] -= position.x() + moveSpeed / dt;
        viewPosition[1] -= position.y() + moveSpeed / dt;
    }

    void Camera2D::apply() {
        ViewProjections::update(get<Camera2dComponent>()->viewProjection);
    }

    void Camera2D::applyZoom(const ZoomType &zoomType) {
        zoom(zoomType);
        apply();
    }

    void Camera2D::applyZoom(const ZoomType &zoomType, float zoomSpeed) {
        zoom(zoomType, zoomSpeed);
        apply();
    }

    void Camera2D::applyMove(const MoveType &moveType) {
        move(moveType);
        apply();
    }

    void Camera2D::applyMove(const MoveType &moveType, float moveSpeed) {
        move(moveType, moveSpeed);
        apply();
    }

    void Camera2D::applyRotate(const RotateType &rotateType) {
        rotate(rotateType);
        apply();
    }

    void Camera2D::applyRotate(const RotateType &rotateType, float rotateSpeed) {
        rotate(rotateType, rotateSpeed);
        apply();
    }

    glm::quat Camera2D::getOrientation() const {
        auto rot = get<Camera2dComponent>()->viewProjection.viewMatrix.rotation;
        return { glm::vec3(-rot, -rot, 0) };
    }

    void Camera3D::create(f32 aspectRatio) {
        add<Camera3dComponent>(Camera3dComponent(aspectRatio));
    }

    PerspectiveMatrix &Camera3D::getPerspective() {
        return get<Camera3dComponent>()->viewProjection.perspectiveMatrix;
    }

    void Camera3D::applyPerspective() {
        math::update(getPerspective());
    }

    ViewMatrix3d& Camera3D::getView() {
        return get<Camera3dComponent>()->viewProjection.viewMatrix;
    }

    void Camera3D::applyView() {
        math::update(getView());
    }

    void Camera3D::onUpdate(time::Time dt) {
        this->dt = dt;
    }

    void Camera3D::zoom(const ZoomType &zoomType) {
        zoom(zoomType, zoomSpeed);
    }

    void Camera3D::move(const MoveType &moveType) {
        move(moveType, moveSpeed);
    }

    void Camera3D::rotate(const RotateType &rotateType) {
        rotate(rotateType, rotateSpeed);
    }

    void Camera3D::setAspectRatio(u32 width, u32 height) {
        getPerspective().aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
        apply();
    }

    void Camera3D::setPosition(const vec3f &position) {
        getView().position.value = position;
        apply();
    }

    void Camera3D::zoom(const ZoomType &zoomType, float zoomSpeed) {
        ENGINE_INFO("{0}: zoom()", tag);
        auto &viewPosition = getView().position.value;
        switch (zoomType) {
            case ZoomType::ZOOM_IN:
                viewPosition[2] += zoomSpeed / dt;
                break;
            case ZoomType::ZOOM_OUT:
                viewPosition[2] -= zoomSpeed / dt;
                break;
        }
    }

    void Camera3D::move(const MoveType &moveType, float moveSpeed) {
        ENGINE_INFO("{0}: move()", tag);
        auto &viewPosition = getView().position.value;
        switch (moveType) {
            case MoveType::DOWN:
                ENGINE_INFO("Camera moved: DOWN");
                viewPosition[1] -= moveSpeed / dt;
                break;
            case MoveType::UP:
                ENGINE_INFO("Camera moved: UP");
                viewPosition[1] += moveSpeed / dt;
                break;
            case MoveType::LEFT:
                ENGINE_INFO("Camera moved: LEFT");
                viewPosition[0] -= moveSpeed / dt;
                break;
            case MoveType::RIGHT:
                ENGINE_INFO("Camera moved: RIGHT");
                viewPosition[0] += moveSpeed / dt;
                break;
            case MoveType::RIGHT_DOWN:
                ENGINE_INFO("Camera moved: RIGHT_DOWN");
                viewPosition[0] += moveSpeed / dt;
                viewPosition[1] -= moveSpeed / dt;
                break;
            case MoveType::RIGHT_UP:
                ENGINE_INFO("Camera moved: RIGHT_UP");
                viewPosition[0] += moveSpeed / dt;
                viewPosition[1] += moveSpeed / dt;
                break;
            case MoveType::LEFT_DOWN:
                ENGINE_INFO("Camera moved: LEFT_DOWN");
                viewPosition[0] -= moveSpeed / dt;
                viewPosition[1] -= moveSpeed / dt;
                break;
            case MoveType::LEFT_UP:
                ENGINE_INFO("Camera moved: LEFT_UP");
                viewPosition[0] -= moveSpeed / dt;
                viewPosition[1] += moveSpeed / dt;
                break;
        }
    }

    void Camera3D::rotate(const RotateType &rotateType, float rotateSpeed) {
        ENGINE_INFO("{0}: rotate()", tag);
        auto& pitch = getView().pitch;
        auto& yaw = getView().yaw;
        auto& roll = getView().roll;
        switch (rotateType) {
            case RotateType::LEFT_X:
                ENGINE_INFO("Camera rotate: LEFT_X");
                pitch -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Y:
                ENGINE_INFO("Camera rotate: LEFT_Y");
                yaw -= rotateSpeed / dt;
                break;
            case RotateType::LEFT_Z:
                ENGINE_INFO("Camera rotate: LEFT_Z");
                roll -= rotateSpeed / dt;
                break;
            case RotateType::RIGHT_X:
                ENGINE_INFO("Camera rotate: RIGHT_X");
                pitch += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Y:
                ENGINE_INFO("Camera rotate: RIGHT_Y");
                yaw += rotateSpeed / dt;
                break;
            case RotateType::RIGHT_Z:
                ENGINE_INFO("Camera rotate: RIGHT_Z");
                roll += rotateSpeed / dt;
                break;
        }
    }

    void Camera3D::move(const vec3f &position) {
        auto &viewPosition = getView().position.value;
        viewPosition[0] -= position.x() + moveSpeed / dt;
        viewPosition[1] -= position.y() + moveSpeed / dt;
        viewPosition[2] -= position.z() + moveSpeed / dt;
    }

    void Camera3D::applyZoom(const ZoomType &zoomType) {
        zoom(zoomType);
        apply();
    }

    void Camera3D::applyZoom(const ZoomType &zoomType, float zoomSpeed) {
        zoom(zoomType, zoomSpeed);
        apply();
    }

    void Camera3D::applyMove(const MoveType &moveType) {
        move(moveType);
        apply();
    }

    void Camera3D::applyMove(const MoveType &moveType, float moveSpeed) {
        move(moveType, moveSpeed);
        apply();
    }

    void Camera3D::applyRotate(const RotateType &rotateType) {
        rotate(rotateType);
        apply();
    }

    void Camera3D::applyRotate(const RotateType &rotateType, float rotateSpeed) {
        rotate(rotateType, rotateSpeed);
        apply();
    }

    void Camera3D::apply() {
        applyPosition();
        ViewProjections::update(get<Camera3dComponent>()->viewProjection);
    }

    void Camera3D::applyMouseRotate() {
        auto delta = event::Input::getMouseDelta();
        float yawSign = getView().upDirection().y() < 0 ? -1.0f : 1.0f;
        auto pitch = delta.y * rotateSpeed;
        auto yaw = yawSign * delta.x * rotateSpeed;
        getView().pitch += pitch;
        getView().yaw += yaw;
        ENGINE_INFO("applyMouseRotate: delta: {0},{1} yawSign: {2} rotX: {3} rotY: {4}",
                    delta.x, delta.y, yawSign, pitch, yaw);
        apply();
    }

    void Camera3D::applyMouseZoom() {
        auto delta = event::Input::getMouseDelta();
        distance -= delta.y * zoomSpeed;
        if (distance < 1) {
            focalPoint += getView().forwardDirection();
            distance = 1;
        }
        apply();
    }

    void Camera3D::applyMouseMove() {
        auto delta = event::Input::getMouseDelta();
        focalPoint += -getView().rightDirection() * delta.x * moveSpeed * distance;
        focalPoint += getView().upDirection() * delta.y * moveSpeed * distance;
        apply();
    }

    vec2f Camera3D::panSpeed() {
        float x = std::min(1024 / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(768 / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    void Camera3D::applyPosition() {
        getView().position.value = focalPoint - getView().forwardDirection() * distance;
    }

}