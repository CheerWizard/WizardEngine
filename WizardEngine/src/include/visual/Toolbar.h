#pragma once

#include <visual/Visual.h>
#include <graphics/core/texture/Texture.h>

namespace engine::visual {

    class ENGINE_API Toolbar final {

    public:
        Toolbar();
        ~Toolbar();

    public:
        static Toolbar& get() {
            static Toolbar instance;
            return instance;
        }

        void draw();

    public:
        bool enabled = false;

    private:
        void onScenePlay();
        void onSceneStop();
        void onSceneStep();
        void onSceneSimulate();

        u32 playIcon = invalidTextureId;
        u32 stopIcon = invalidTextureId;
        u32 pauseIcon = invalidTextureId;
        u32 simulateIcon = invalidTextureId;
        u32 stepIcon = invalidTextureId;
    };

}
