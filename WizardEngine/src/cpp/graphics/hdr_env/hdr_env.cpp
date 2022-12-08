#include <graphics/hdr_env/hdr_env.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void HdrEnv::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "HdrEnv";

        yaml::serialize(out, "type", static_cast<u32>(type));
        transform.serialize(out);
        texture.serialize(out);

        out << YAML::EndMap;
    }

    void HdrEnv::deserialize(const YAML::Node &parent) {
        auto root = parent["HdrEnv"];
        if (root) {
            type = static_cast<HdrEnvType>(root["type"].as<u32>());
            transform.deserialize(parent);
            texture.deserialize(parent);
            init();
        }
    }

    void HdrEnvRenderer::init() {
        generateCubemapShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "generate_cubemap",
                        "v_hdr_env.glsl",
                        "f_hdr_env.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        generateCubemapShader.bindVertexFormat();

        cubemapShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "cubemap",
                        "v_hdr_skycube.glsl",
                        "f_hdr_skycube.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader()
        );
        cubemapShader.bindVertexFormat();

        frameBuffer = createRef<FrameBuffer>();
        FrameBufferFormat frameBufferFormat { 512, 512, 1 };
        frameBufferFormat.renderBufferAttachment = { DepthFormat::DEPTH24 };
        frameBuffer->updateFormat(frameBufferFormat);
        u32 cubemap = TextureBuffer::generateCubeMap(
                512, 512,
                ColorFormat::RGB16F, ColorFormat::RGB, PixelsType::FLOAT
        );
        TextureBuffer::setDefaultParamsCubeMap(cubemap);
    }

    void HdrEnvRenderer::release() {
        renderModel.release();
        cubemapShader.destroy();
        generateCubemapShader.destroy();
        frameBuffer->destroy();
    }

    void HdrEnvRenderer::upload(HdrEnv* hdrEnv) {
        auto geometry = hdrEnv->geometry;
        if (geometry.vertexData.values) {
            ENGINE_INFO("uploadStatic HDR env to renderer");
            if (!renderModel.hasCapacity(geometry)) {
                renderModel = { 0, geometry.vertexData.size };
                renderModel.vao.bind();
                renderModel.vbo.setFormat(generateCubemapShader.getVertexFormat());
                VertexArray::unbind();
            }
            renderModel.uploadStatic(geometry);
            delete geometry.vertexData.values;
        } else {
            ENGINE_WARN("HDR env is already uploaded!");
        }

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] = {
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };
        // we need to render once HDR env into cubemap texture and capture it into frame
        // convert HDR environment map to cubemap equivalent
        generateCubemapShader.start();
        // update textures
        auto texture = hdrEnv->texture;
        generateCubemapShader.setUniform(texture.sampler);
        TextureBuffer::activate(texture.sampler.value);
        TextureBuffer::bind(texture.textureId, texture.typeId);
        // update transform
        generateCubemapShader.setUniform(hdrEnv->transform.modelMatrix);
        // update VP
        GLMMat4fUniform captureProjectionUniform("projection", captureProjection);
        generateCubemapShader.setUniform(captureProjectionUniform);
        frameBuffer->bind();
        for (u32 i = 0; i < 6; i++) {
            GLMMat4fUniform viewUniform("view", captureViews[i]);
            generateCubemapShader.setUniform(viewUniform);
            frameBuffer->attachCubeMap(i, cubemap);
            graphics::clearBuffer(BufferBit::COLOR | BufferBit::DEPTH);
            // draw call
            renderModel.vao.bind();
            drawV(hdrEnv->geometry.drawType, hdrEnv->geometry.vertexData.size);
        }
        generateCubemapShader.stop();

        FrameBuffer::bindDefault();
    }

    void HdrEnvRenderer::render(const ecs::Entity& entity, Camera3D& camera) {
        ENGINE_INFO("SkyboxRenderer::render()");
        HdrEnv *hdrEnv = entity.get<HdrEnv>();
        if (!cubemapShader.isReady() || !hdrEnv) return;

        cubemapShader.start();
        // update cubemap texture
        IntUniform cubemapSampler { "cubemap", 0 };
        cubemapShader.setUniform(cubemapSampler);
        TextureBuffer::activate(0);
        TextureBuffer::bind(cubemap, TextureType::TEXTURE_2D);
        // update VP
        cubemapShader.setUniform(camera.getPerspective());
        cubemapShader.setUniform(camera.getView());
        // update transform
        cubemapShader.setUniform(hdrEnv->transform.modelMatrix);
        // draw call
        renderModel.vao.bind();
        drawV(hdrEnv->geometry.drawType, hdrEnv->geometry.vertexData.size);
        cubemapShader.stop();
    }
}