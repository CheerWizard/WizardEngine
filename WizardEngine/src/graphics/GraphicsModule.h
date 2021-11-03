//
// Created by mecha on 01.11.2021.
//

#pragma once

#include "core/RenderSystem.h"
#include "GraphicsFactory.h"

#include "core/sources/MeshSource.h"
#include "core/sources/TextureSource.h"
#include "core/sources/ShaderSource.h"

namespace engine {

    class GraphicsModule {

    public:
        GraphicsModule() = default;
        virtual ~GraphicsModule() = default;

    public:
        // creates graphics context for incoming native window!
        virtual void createContext(void* nativeWindow) = 0;
        virtual Ref<GraphicsFactory> newGraphicsFactory() = 0;

    public:
        Scope<RenderSystem> newRenderSystem(
                const Ref<TextureSource> &textureSource,
                const Scope<ShaderSource> &shaderSource,
                const Ref<FrameController> &frameController
        );
        Scope<MeshSource> newMeshSource();
        Ref<TextureSource> newTextureSource();
        Scope<ShaderSource> newShaderSource();
        Ref<FrameController> newFrameController();
        Ref<Renderer> newTextureRenderer(const Ref<Shader> &shader, const Ref<TextureSource> &textureSource);
        Ref<RenderSettings> newRenderSettings();

    protected:
        void provideShaderControllers(Ref<Renderer> &renderer, const Ref<TextureSource> &textureSource);

    protected:
        Ref<GraphicsFactory> graphicsFactory = nullptr;

    };

}