#pragma once

#include <ecs/Entity.h>
#include <graphics/camera/CameraComponents.h>

#include <time/Time.h>
#include <core/Memory.h>
#include <event/KeyCodes.h>

#include <unordered_map>

#include <glm/gtx/quaternion.hpp>

#define DEFAULT_CAMERA_ZOOM_SPEED 0.001f
#define DEFAULT_CAMERA_MOVE_SPEED 0.001f
#define DEFAULT_CAMERA_ROTATE_SPEED 0.001f
#define NONE_TYPE(T) static_cast<T>(0)

namespace engine::graphics {

    using namespace math;

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

    class ENGINE_API Camera2D : public ecs::Entity {

    public:
        Camera2D() = default;

        Camera2D(const std::string &tag, ecs::EntityContainer* container) : ecs::Entity(tag, container) {
            create();
        }

        Camera2D(ecs::EntityContainer* container) : ecs::Entity("Camera", container) {
            create();
        }

        ~Camera2D() = default;

    public:
        void onUpdate(time::Time dt);

    public:
        void zoom(const ZoomType &zoomType);
        void zoom(const ZoomType &zoomType, float zoomSpeed);
        void move(const MoveType &moveType);
        void move(const MoveType &moveType, float moveSpeed);
        void move(const vec2f& position);
        void rotate(const RotateType &rotateType);
        void rotate(const RotateType &rotateType, float rotateSpeed);
        void setPosition(const vec2f &position);
        vec3f& getPosition();

        void applyZoom(const ZoomType &zoomType);
        void applyZoom(const ZoomType &zoomType, float zoomSpeed);

        void applyMove(const MoveType &moveType);
        void applyMove(const MoveType &moveType, float moveSpeed);

        void applyRotate(const RotateType &rotateType);
        void applyRotate(const RotateType &rotateType, float rotateSpeed);

        glm::quat getOrientation() const;

    public:
        OrthographicMatrix& getOrtho();
        void applyOrtho();
        ViewMatrix2d& getView();
        void applyView();
        void apply();

    private:
        void create();

    public:
        float zoomSpeed = DEFAULT_CAMERA_ZOOM_SPEED;
        float moveSpeed = DEFAULT_CAMERA_MOVE_SPEED;
        float rotateSpeed = DEFAULT_CAMERA_ROTATE_SPEED;

    private:
        time::Time dt;
        const char* tag = "Camera2D";

    };

    class ENGINE_API Camera3D : public ecs::Entity {

    public:
        Camera3D() = default;

        Camera3D(const std::string &tag, const float& aspectRatio, ecs::EntityContainer* container) : ecs::Entity(tag, container) {
            create(aspectRatio);
        }

        Camera3D(const float& aspectRatio, ecs::EntityContainer* container) : ecs::Entity("Camera", container) {
            create(aspectRatio);
        }

        ~Camera3D() = default;

    public:
        void onUpdate(time::Time dt);

    public:
        void zoom(const ZoomType &zoomType);
        void zoom(const ZoomType &zoomType, float zoomSpeed);
        void move(const MoveType &moveType);
        void move(const MoveType &moveType, float moveSpeed);
        void move(const vec3f& position);
        void rotate(const RotateType &rotateType);
        void rotate(const RotateType &rotateType, float rotateSpeed);
        void setPosition(const vec3f &position);
        vec3f& getPosition();

        void applyZoom(const ZoomType &zoomType);
        void applyZoom(const ZoomType &zoomType, float zoomSpeed);

        void applyMove(const MoveType &moveType);
        void applyMove(const MoveType &moveType, float moveSpeed);

        void applyRotate(const RotateType &rotateType);
        void applyRotate(const RotateType &rotateType, float rotateSpeed);

        void applyMouseZoom();
        void applyMouseMove();
        void applyMouseRotate();

        vec2f panSpeed();

        void applyPosition();

    public:
        PerspectiveMatrix& getPerspective();
        void applyPerspective();
        ViewMatrix3d& getView();
        void applyView();
        ViewProjection3d& getViewProjection();
        void setAspectRatio(u32 width, u32 height);
        void apply();

    private:
        void create(f32 aspectRatio);

    public:
        float zoomSpeed = DEFAULT_CAMERA_ZOOM_SPEED;
        float moveSpeed = DEFAULT_CAMERA_MOVE_SPEED;
        float rotateSpeed = DEFAULT_CAMERA_ROTATE_SPEED;
        float distance = 10.0f;
        vec3f focalPoint;

    private:
        time::Time dt;
        const char* tag = "Camera3D";

    };

}