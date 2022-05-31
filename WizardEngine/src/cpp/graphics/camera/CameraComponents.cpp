//
// Created by mecha on 21.05.2022.
//

#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void Camera2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Camera2dComponent";
        out << YAML::BeginMap; // Camera2dComponent

        out << YAML::Key << "ViewProjection2d" << YAML::Value;
        out << YAML::BeginMap; // ViewProjection2d
        yaml::serialize(out, "Name", std::string(viewProjection.name));

        auto& o = viewProjection.orthographicMatrix;
        out << YAML::Key << "OrthographicProjection" << YAML::Value;
        out << YAML::BeginMap; // OrthographicProjection
        yaml::serialize(out, "Name", std::string(o.name));
        yaml::serialize(out, "Left", o.left);
        yaml::serialize(out, "Top", o.top);
        yaml::serialize(out, "Right", o.right);
        yaml::serialize(out, "Bottom", o.bottom);
        out << YAML::EndMap; // OrthographicProjection

        auto& vm = viewProjection.viewMatrix;
        out << YAML::Key << "ViewMatrix2d" << YAML::Value;
        out << YAML::BeginMap; // ViewMatrix2d
        yaml::serialize(out, "Name", std::string(vm.name));
        yaml::serialize(out, "Position", vm.position);
        yaml::serialize(out, "Rotation", vm.rotation);
        out << YAML::EndMap; // ViewMatrix2d

        out << YAML::EndMap; // ViewProjection2d

        out << YAML::EndMap; // Camera2dComponent
    }

    void Camera2dComponent::deserialize(const YAML::Node &parent) {
        auto camera = parent["Camera2dComponent"];
        if (camera) {
            auto vpNode = camera["ViewProjection2d"];
            viewProjection.name = vpNode["Name"].as<std::string>().c_str();

            auto oNode = vpNode["OrthographicProjection"];
            viewProjection.orthographicMatrix.name = oNode["Name"].as<std::string>().c_str();
            viewProjection.orthographicMatrix.left = oNode["Left"].as<f32>();
            viewProjection.orthographicMatrix.top = oNode["Top"].as<f32>();
            viewProjection.orthographicMatrix.right = oNode["Right"].as<f32>();
            viewProjection.orthographicMatrix.bottom = oNode["Bottom"].as<f32>();

            auto vmNode = vpNode["ViewMatrix2d"];
            viewProjection.viewMatrix.name = vmNode["Name"].as<std::string>().c_str();
            viewProjection.viewMatrix.position = vmNode["Position"].as<glm::fvec3>();
            viewProjection.viewMatrix.rotation = vmNode["Rotation"].as<f32>();
        }
    }

    void Camera3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Camera3dComponent";
        out << YAML::BeginMap; // Camera3dComponent

        out << YAML::Key << "ViewProjection3d" << YAML::Value;
        out << YAML::BeginMap; // ViewProjection3d
        yaml::serialize(out, "Name", std::string(viewProjection.name));

        auto& p = viewProjection.perspectiveMatrix;
        out << YAML::Key << "PerspectiveProjection" << YAML::Value;
        out << YAML::BeginMap; // PerspectiveProjection
        yaml::serialize(out, "Name", std::string(p.name));
        yaml::serialize(out, "AspectRatio", p.aspectRatio);
        yaml::serialize(out, "FOV", p.fieldOfView);
        yaml::serialize(out, "ZFar", p.zFar);
        yaml::serialize(out, "ZNear", p.zNear);
        out << YAML::EndMap; // PerspectiveProjection

        auto& vm = viewProjection.viewMatrix;
        out << YAML::Key << "ViewMatrix3d" << YAML::Value;
        out << YAML::BeginMap; // ViewMatrix3d
        yaml::serialize(out, "Name", std::string(vm.name));
        yaml::serialize(out, "Position", vm.position.value);
        yaml::serialize(out, "Rotation", vm.rotation);
        yaml::serialize(out, "Scale", vm.scale);
        out << YAML::EndMap; // ViewMatrix3d

        out << YAML::EndMap; // ViewProjection3d

        out << YAML::EndMap; // Camera3dComponent
    }

    void Camera3dComponent::deserialize(const YAML::Node &parent) {
        auto camera = parent["Camera3dComponent"];
        if (camera) {
            auto vpNode = camera["ViewProjection3d"];
            viewProjection.name = vpNode["Name"].as<std::string>().c_str();

            auto pNode = vpNode["PerspectiveProjection"];
            viewProjection.perspectiveMatrix.name = pNode["Name"].as<std::string>().c_str();
            viewProjection.perspectiveMatrix.aspectRatio = pNode["AspectRatio"].as<f32>();
            viewProjection.perspectiveMatrix.fieldOfView = pNode["FOV"].as<f32>();
            viewProjection.perspectiveMatrix.zFar = pNode["ZFar"].as<f32>();
            viewProjection.perspectiveMatrix.zNear = pNode["ZNear"].as<f32>();

            auto vmNode = vpNode["ViewMatrix3d"];
            viewProjection.viewMatrix.name = vmNode["Name"].as<std::string>().c_str();
            viewProjection.viewMatrix.position.value = vmNode["Position"].as<glm::fvec3>();
            viewProjection.viewMatrix.rotation = vmNode["Rotation"].as<glm::fvec3>();
            viewProjection.viewMatrix.scale = vmNode["Scale"].as<f32>();
        }
    }
}