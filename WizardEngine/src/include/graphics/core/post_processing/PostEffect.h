//
// Created by mecha on 30.08.2022.
//

#pragma once

#include <graphics/core/Renderer.h>
#include <platform/graphics/FrameBuffer.h>
#include <platform/graphics/tools/VideoStats.h>

namespace engine::graphics {

    using namespace math;

    struct PostEffect {
        const char* name = nullptr;
        bool enabled = false; // used to enable/disable rendering for this post effect
        // primitives
        vector<FloatUniform> floats;
        vector<BoolUniform> flags;
        vector<IntUniform> ints;
        vector<DoubleUniform> doubles;
        // vectors
        vector<Vec4fUniform> vec4;
        // textures
        vector<TextureComponent> textures;

        PostEffect() = default;
        PostEffect(const char* name) : name(name) {}

        [[nodiscard]] size_t getLimit() const {
            return VideoStats::getMaxSlots() / textures.size();
        }
    };

    class PostEffectRenderer {

    public:
        PostEffectRenderer() = default;
        PostEffectRenderer(const BaseShaderProgram& shaderProgram, const Ref<FrameBuffer>& frameBuffer)
        : shaderProgram(shaderProgram), frameBuffer(frameBuffer) {}

    public:
        void release();
        // returns textureId of rendered post effect texture
        virtual u32 render(u32 textureId);

    public:
        inline Ref<FrameBuffer>& getFrameBuffer() {
            return frameBuffer;
        }

        inline Ref<PostEffect>& getPostEffect() {
            return postEffect;
        }

    protected:
        void updatePostEffect();

    protected:
        Ref<FrameBuffer> frameBuffer; // shared memory as it's used/modified from Application system
        Ref<PostEffect> postEffect; // shared memory as it's used/modified from Application system
        BaseShaderProgram shaderProgram;
    };

}