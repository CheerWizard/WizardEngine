//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <graphics/core/shader/Uniform.h>

namespace engine::visual {

    using namespace shader;
    using namespace math;

    class ENGINE_API Controller final {
    public:
        static void draw(Vec2fUniform& uniform, float reset, float width);
        static void draw(Vec3fUniform& uniform, float reset, float width);
        static void draw(Vec4fUniform& uniform, float reset, float width);
    };

    class ENGINE_API Checkbox final {
    public:
        static void draw(BoolUniform& uniform);
        static void draw(const char* name, bool& value);
    };

    class ENGINE_API Slider final {
    public:
        static void draw(FloatUniform& uniform, const vec2f& range);
        static void draw(IntUniform& uniform, const vec2i& range);
        static void draw(DoubleUniform& uniform, const vec2f& range);

        static void draw(const char* name, float& value, const vec2f& range);
        static void draw(const char* name, int& value, const vec2i& range);
        static void draw(const char* name, u32& value, const vec2i& range);
    };

    class ENGINE_API Text final {
    public:
        static void draw(const char* text);
    };

}
