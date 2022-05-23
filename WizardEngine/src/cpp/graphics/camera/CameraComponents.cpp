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

}