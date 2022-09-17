//
// Created by mecha on 30.08.2022.
//

#include <graphics/core/post_processing/PostEffect.h>

namespace engine::graphics {

    struct ENGINE_API HdrEffect : PostEffect {

        HdrEffect(const char* name = "hdrEffect") : PostEffect(name) {
            floats.emplace_back("gamma", 2.2);
            floats.emplace_back("exposure", 0.5);
        }

        inline float& gamma() {
            return floats[0].value;
        }

        inline float& exposure() {
            return floats[1].value;
        }

    };

    class ENGINE_API HdrEffectRenderer : public PostEffectRenderer {

    public:
        HdrEffectRenderer() : PostEffectRenderer() {}
        HdrEffectRenderer(const FrameBufferFormat& frameBufferFormat);

    public:
        inline Ref<HdrEffect>& getHdrEffect() {
            return (Ref<HdrEffect>&)(postEffect);
        }
    };

    struct ENGINE_API KernelEffect : PostEffect {

        KernelEffect(const char* name = "kernelEffect") : PostEffect(name) {
            floats.emplace_back("offset", 1.0 / 300.0);
        }

        inline float& offset() {
            return floats[0].value;
        }

    };

    struct ENGINE_API BlurEffect : KernelEffect {
        BlurEffect(const char* name = "blurEffect") : KernelEffect(name) {}
    };

    struct ENGINE_API SharpenEffect : KernelEffect {
        SharpenEffect(const char* name = "sharpenEffect") : KernelEffect(name) {}
    };

    struct ENGINE_API EdgeDetectionEffect : KernelEffect {
        EdgeDetectionEffect(const char* name = "edgeDetectionEffect") : KernelEffect(name) {}
    };

    class ENGINE_API BlurEffectRenderer : public PostEffectRenderer {

    public:
        BlurEffectRenderer() : PostEffectRenderer() {}
        BlurEffectRenderer(const FrameBufferFormat& frameBufferFormat);

    public:
        inline Ref<BlurEffect>& getBlurEffect() {
            return (Ref<BlurEffect>&)(postEffect);
        }
    };

    class ENGINE_API SharpenEffectRenderer : public PostEffectRenderer {

    public:
        SharpenEffectRenderer() : PostEffectRenderer() {}
        SharpenEffectRenderer(const FrameBufferFormat& frameBufferFormat);

    public:
        inline Ref<SharpenEffect>& getSharpenEffect() {
            return (Ref<SharpenEffect>&)(postEffect);
        }
    };

    class ENGINE_API EdgeDetectionEffectRenderer : public PostEffectRenderer {

    public:
        EdgeDetectionEffectRenderer() : PostEffectRenderer() {}
        EdgeDetectionEffectRenderer(const FrameBufferFormat& frameBufferFormat);

    public:
        inline Ref<EdgeDetectionEffect>& getEdgeDetectionEffect() {
            return (Ref<EdgeDetectionEffect>&)(postEffect);
        }
    };

    struct ENGINE_API GaussianBlurEffect : PostEffect {
        u32 amount = 10;
        GaussianBlurEffect(const char* name = "gaussianBlurEffect") : PostEffect(name) {}
    };

    class ENGINE_API GaussianBlurEffectRenderer : public PostEffectRenderer {

    public:
        GaussianBlurEffectRenderer() : PostEffectRenderer() {}
        GaussianBlurEffectRenderer(const FrameBufferFormat& frameBufferFormat);

    public:
        inline Ref<GaussianBlurEffect>& getGaussianBlurEffect() {
            return (Ref<GaussianBlurEffect>&)(postEffect);
        }

        u32 render(u32 textureId) override;

    private:
        Ref<FrameBuffer> horizontalFrameBuffer;
    };

}