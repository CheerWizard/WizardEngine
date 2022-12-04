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

    class ENGINE_API CustomCheckbox final {
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
        static void label(const char* text, const vec2f& padding = { 1, 1 });
        static bool field(const char* id, const char* title, std::string* text,
                          const vec2f& padding = { 1, 1 }, bool alignLeft = true);
        static void centered(const char* text, float maxWidth, int lineId = 0, bool separator = false);
    };

    class ENGINE_API Line final {
    public:
        static void draw(int id);
        static void draw(const std::string& id);
    };

    class ENGINE_API Button final {
    public:
        static bool textButton(const char* text, const vec2f& size,
                               float paddingTop = 0, float paddingBottom = 0, float paddingLeft = 0, float paddingRight = 0);
    };

    class ENGINE_API ColorPicker final {
    public:
        static bool draw(Vec4fUniform& colorUniform);
    };

}
