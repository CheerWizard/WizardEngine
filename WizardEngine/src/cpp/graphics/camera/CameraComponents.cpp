//
// Created by mecha on 21.05.2022.
//

#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void Camera2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Camera2dComponent";
        out << YAML::BeginMap; // Camera2dComponent

        out << YAML::Key << "viewProjection2d" << YAML::Value;
        out << YAML::BeginMap; // ViewProjection2d
        yaml::serialize(out, "name", std::string(viewProjection.name));

        auto& o = viewProjection.orthographicMatrix;
        out << YAML::Key << "orthographicProjection" << YAML::Value;
        out << YAML::BeginMap; // OrthographicProjection
        yaml::serialize(out, "name", std::string(o.name));
        yaml::serialize(out, "left", o.left);
        yaml::serialize(out, "top", o.top);
        yaml::serialize(out, "right", o.right);
        yaml::serialize(out, "bottom", o.bottom);
        out << YAML::EndMap; // OrthographicProjection

        auto& vm = viewProjection.viewMatrix;
        out << YAML::Key << "viewMatrix2d" << YAML::Value;
        out << YAML::BeginMap; // ViewMatrix2d
        yaml::serialize(out, "name", std::string(vm.name));
        yaml::serialize(out, "position", vm.position);
        yaml::serialize(out, "rotation", vm.rotation);
        out << YAML::EndMap; // ViewMatrix2d

        out << YAML::EndMap; // ViewProjection2d

        out << YAML::EndMap; // Camera2dComponent
    }

    void Camera2dComponent::deserialize(const YAML::Node &parent) {
        auto camera = parent["Camera2dComponent"];
        if (camera) {
            auto vpNode = camera["viewProjection2d"];
            viewProjection.name = vpNode["name"].as<std::string>().c_str();

            auto oNode = vpNode["orthographicProjection"];
            viewProjection.orthographicMatrix.name = oNode["name"].as<std::string>().c_str();
            viewProjection.orthographicMatrix.left = oNode["left"].as<f32>();
            viewProjection.orthographicMatrix.top = oNode["top"].as<f32>();
            viewProjection.orthographicMatrix.right = oNode["right"].as<f32>();
            viewProjection.orthographicMatrix.bottom = oNode["bottom"].as<f32>();

            auto vmNode = vpNode["viewMatrix2d"];
            viewProjection.viewMatrix.name = vmNode["name"].as<std::string>().c_str();
            viewProjection.viewMatrix.position = vmNode["position"].as<glm::fvec3>();
            viewProjection.viewMatrix.rotation = vmNode["rotation"].as<f32>();
        }
    }

    void Camera3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Camera3dComponent";
        out << YAML::BeginMap; // Camera3dComponent

        out << YAML::Key << "viewProjection3d" << YAML::Value;
        out << YAML::BeginMap; // ViewProjection3d
        yaml::serialize(out, "name", std::string(viewProjection.name));

        auto& p = viewProjection.perspectiveMatrix;
        out << YAML::Key << "perspectiveProjection" << YAML::Value;
        out << YAML::BeginMap; // PerspectiveProjection
        yaml::serialize(out, "name", std::string(p.name));
        yaml::serialize(out, "aspectRatio", p.aspectRatio);
        yaml::serialize(out, "fov", p.fieldOfView);
        yaml::serialize(out, "zFar", p.zFar);
        yaml::serialize(out, "zNear", p.zNear);
        out << YAML::EndMap; // PerspectiveProjection

        auto& vm = viewProjection.viewMatrix;
        out << YAML::Key << "viewMatrix3d" << YAML::Value;
        out << YAML::BeginMap; // ViewMatrix3d
        yaml::serialize(out, "name", std::string(vm.name));
        yaml::serialize(out, "position", vm.position.value);
        yaml::serialize(out, "rotation", vm.rotation);
        yaml::serialize(out, "scale", vm.scale);
        out << YAML::EndMap; // ViewMatrix3d

        out << YAML::EndMap; // ViewProjection3d

        out << YAML::EndMap; // Camera3dComponent
    }

    void Camera3dComponent::deserialize(const YAML::Node &parent) {
        auto camera = parent["Camera3dComponent"];
        if (camera) {
            auto vpNode = camera["viewProjection3d"];
            viewProjection.name = vpNode["name"].as<std::string>().c_str();

            auto pNode = vpNode["perspectiveProjection"];
            viewProjection.perspectiveMatrix.name = pNode["name"].as<std::string>().c_str();
            viewProjection.perspectiveMatrix.aspectRatio = pNode["aspectRatio"].as<f32>();
            viewProjection.perspectiveMatrix.fieldOfView = pNode["fov"].as<f32>();
            viewProjection.perspectiveMatrix.zFar = pNode["zFar"].as<f32>();
            viewProjection.perspectiveMatrix.zNear = pNode["zNear"].as<f32>();

            auto vmNode = vpNode["viewMatrix3d"];
            viewProjection.viewMatrix.name = vmNode["name"].as<std::string>().c_str();
            viewProjection.viewMatrix.position.value = vmNode["position"].as<glm::fvec3>();
            viewProjection.viewMatrix.rotation = vmNode["rotation"].as<glm::fvec3>();
            viewProjection.viewMatrix.scale = vmNode["scale"].as<f32>();
        }
    }
}