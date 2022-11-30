#pragma once

#include <visual/Visual.h>
#include <graphics/materials/Material.h>

namespace engine::visual {

    using namespace engine::graphics;

    class ENGINE_API MaterialPanel final {
    public:
        static void draw(Material& material);
    };

}
