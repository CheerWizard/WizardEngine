//
// Created by mecha on 04.11.2021.
//

#include "Light.h"

namespace engine {

    void PhongLight::create() {
        add<PhongLightComponent>(PhongLightComponent());
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