//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/vector.h>
#include <core/Memory.h>
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

        static void load(const char* filePath);

    private:
        ALuint id;
    };

    class Buffers {

    private:
        Buffers();
        ~Buffers();

    public:
        static Ref<Buffers>& get();

    private:
        vector<Buffer> buffers;

    };

}
