//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/vector.h>
#include <core/Memory.h>
#include <core/primitives.h>

#include <AL/al.h>

namespace engine::audio {

    using namespace core;

    class Buffer final {

    public:
        Buffer() = default;
        ~Buffer() = default;

    public:
        void create();
        void destroy();
        void recreate();

        void bind() const;
        static void unbind();

        void load(const AudioData &audioData) const;

    private:
        u32 id = 0;
    };

}
