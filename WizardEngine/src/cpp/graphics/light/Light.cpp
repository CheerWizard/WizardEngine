//
// Created by mecha on 04.11.2021.
//

#include <graphics/light/Light.h>

namespace engine::graphics {

    void PhongLight::create() {
        add<PhongLightComponent>(PhongLightComponent());
    }

    vec3f& PhongLight::getPosition() {
        return get<PhongLightComponent>()->position.value;
    }

    void PhongLight::apply() {
        get<PhongLightComponent>()->position.isUpdated = true;
    }

    void DirectLight::create() {
        add<DirectLightComponent>(DirectLightComponent());
    }

    void PointLight::create() {
        add<PointLightComponent>(PointLightComponent());
    }

    void FlashLight::create() {
        add<FlashLightComponent>(FlashLightComponent());
    }

}