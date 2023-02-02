#include <graphics/skybox/Skybox.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void Skybox::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Skybox";

        yaml::serialize(out, "type", static_cast<u32>(type));
        transform.serialize(out);
        textures.serialize(out);

        out << YAML::EndMap;
    }

    void Skybox::deserialize(const YAML::Node &parent) {
        auto root = parent["Skybox"];
        if (root) {
            type = static_cast<SkyboxType>(root["type"].as<u32>());
            transform.deserialize(parent);
            textures.deserialize(parent);
            init();
        }
    }

    void Skybox::read(std::fstream &file) {
        ::read(file, type);
        geometry.read(file);
        textures.read(file);
        transform.read(file);
    }

    void Skybox::write(std::fstream &file) {
        ::write(file, type);
        geometry.write(file);
        textures.write(file);
        transform.write(file);
    }

    void SkyboxRenderer::init() {
        shaderProgram = shader::BaseShaderProgram(
                ENGINE_SHADERS_PATH + "/" + "v_cubemap.glsl",
                ENGINE_SHADERS_PATH + "/" + "f_cubemap.glsl"
        );
        shaderProgram.bindVertexFormat();
    }

    void SkyboxRenderer::release() {
        renderModel.release();
        shaderProgram.destroy();
    }

    void SkyboxRenderer::upload(Skybox* skybox) {
        auto skyboxGeometry = skybox->geometry;
        if (skyboxGeometry.vertexData.values) {
            ENGINE_INFO("uploadStatic skybox to renderer");
            if (!renderModel.hasCapacity(skybox->geometry)) {
                renderModel = { 0, skybox->geometry.vertexData.size };
                renderModel.vao.bind();
                renderModel.vbo.setFormat(shaderProgram.getVertexFormat());
                VertexArray::unbind();
            }
            renderModel.uploadStatic(skybox->geometry);
            delete skyboxGeometry.vertexData.values;
        } else {
            ENGINE_WARN("Skybox is already uploaded!");
        }
    }

    void SkyboxRenderer::render(const ecs::Entity& entity, Camera3D& camera) {
        ENGINE_INFO("SkyboxRenderer::render()");
        Skybox* skybox = entity.get<Skybox>();
        if (!shaderProgram.isReady() || !skybox) return;

        shaderProgram.start();
        // update camera
        shaderProgram.getVertexShader().updateUniformBuffer(camera.getViewProjection(), 0);
        // update textures
        auto textures = skybox->textures;
        shaderProgram.setUniform(textures.sampler);
        TextureBuffer::activate(textures.sampler.value);
        TextureBuffer::bind(textures.textureId, textures.typeId);
        // update transform
        shaderProgram.setUniform(skybox->transform.modelMatrix);
        // draw call
        renderModel.vao.bind();
        drawV(skybox->geometry.drawType, skybox->geometry.vertexData.size);
        shaderProgram.stop();
    }

}