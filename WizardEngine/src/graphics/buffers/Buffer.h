//
// Created by mecha on 07.09.2021.
//

#pragma once

namespace engine {

    class Buffer {
    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void load() = 0;

        virtual void onCreate() = 0;
        virtual void onDestroy() = 0;

    protected:
        unsigned int id;
    };

}