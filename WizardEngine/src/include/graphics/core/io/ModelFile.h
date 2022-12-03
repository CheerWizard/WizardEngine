//
// Created by mecha on 29.09.2021.
//

#pragma once

#include <core/filesystem.h>
#include <core/Mappers.h>

#include <graphics/core/geometry/Mesh.h>
#include <graphics/materials/Material.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <sstream>

namespace engine::io {

    using namespace math;
    using namespace graphics;

    struct ENGINE_API Face {
        int posIndex;
        int uvIndex;
        int normalIndex;
    };

    struct ENGINE_API ModelVertex {
        vec3f position = { 0.5f, 0.5f, 0.5f };
        vec2f uv = { 0, 0 };
        vec3f normal = { 0, 0, 0 };
        vec3f tangent = { 0, 0, 0 };
        vec3f bitangent = { 0, 0, 0 };
    };

    struct ENGINE_API ModelMesh : BaseMesh<ModelVertex> {
        Material material;
    };

    struct ENGINE_API Model {
        std::vector<ModelMesh> meshes;
    };

    // const container of aiTextureType and actual const string naming
    class ENGINE_API ModelTextureTypes final {

    private:
        ModelTextureTypes() = default;

    public:
        static const char* get(const aiTextureType& aiTextureType) {
            static ModelTextureTypes instance;
            return instance.textureTypes.at(aiTextureType);
        }

    public:
        const std::unordered_map<aiTextureType, const char*> textureTypes = {
                { aiTextureType::aiTextureType_AMBIENT, "ambient" },
                { aiTextureType::aiTextureType_DIFFUSE, "diffuse" },
                { aiTextureType::aiTextureType_SPECULAR, "specular" },
                { aiTextureType::aiTextureType_NORMALS, "normal" },
        };
    };

    template<typename T>
    struct ModelFileListener {
        std::function<void(const Model&)> success;
        std::function<void(const exception&)> failure;
    };

    enum ModelFileOption {
        triangulate = aiProcess_Triangulate, // divide complex polygons into triangles
        flip_uv = aiProcess_FlipUVs, // we can simply flip stbi_image instead
        calc_tang_space = aiProcess_CalcTangentSpace,
        texture_invert = aiTextureFlags_Invert,
        embed_textures = aiProcess_EmbedTextures // read embedded textures from .fbx, .gltf
    };

    struct ENGINE_API ModelFileOptions {
        vector<ModelFileOption> flags = { triangulate, calc_tang_space, embed_textures };

        [[nodiscard]] u32 getFlag() const {
            if (flags.empty()) return 0;

            u32 flag = flags[0];
            for (u32 i = 1; i < flags.size(); i++) {
                flag = flag | flags[i];
            }
            return flag;
        }
    };

    template<typename T>
    class ModelFile final {

        typedef std::unordered_map<std::string, Model> ModelMap;

    public:
        static void read(const std::string &filepath, const std::string& texturesFilePath, const ModelFileListener<T>& listener, const ModelFileOptions& options = ModelFileOptions());

    private:
        static Model read(const std::string &filePath, const std::string& texturesFilePath, const ModelFileOptions& options);
        static bool exists(const std::string &filepath);
        static void clear();
        static void extractNodes(
                const std::string& texturesFilePath,
                aiNode *node,
                const aiScene *scene,
                std::vector<ModelMesh>& meshes
        );
        static ModelMesh extractMesh(aiMesh *mesh);
        static Material extractMaterial(const std::string& texturesPath, aiMesh *mesh, const aiScene *scene);
        static u32 extractTexture(
                const std::string& texturesPath,
                aiMaterial* material,
                aiTextureType textureType,
                bool& enableTexture
        );

        static Model getFromMemory(const std::string& filepath);
        static Model copyFromMemory(const std::string& filepath);

    private:
        static ModelMap modelMap;
    };

    template<typename T>
    std::unordered_map<std::string, Model> ModelFile<T>::modelMap;

    template<typename T>
    Model ModelFile<T>::read(const std::string &filePath, const std::string& texturesFilePath, const ModelFileOptions& options) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(filePath, options.getFlag());

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::string message = "Assimp: failed to import file '" + filePath + "'; error: " + import.GetErrorString();
            ENGINE_THROW(file_not_found(exception_priority::ERR, message));
        }

        std::vector<ModelMesh> meshes;
        extractNodes(texturesFilePath, scene->mRootNode, scene, meshes);
        return { meshes };
    }

    template<typename T>
    void ModelFile<T>::extractNodes(
            const std::string& texturesFilePath,
            aiNode *node,
            const aiScene *scene,
            std::vector<ModelMesh>& meshes
    ) {
        // extract all meshes and materials of node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            ModelMesh modelMesh = extractMesh(mesh);
            modelMesh.material = extractMaterial(texturesFilePath, mesh, scene);
            meshes.push_back(modelMesh);
        }
        // recursively extract node's children
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            extractNodes(texturesFilePath, node->mChildren[i], scene, meshes);
        }
    }

    template<typename T>
    ModelMesh ModelFile<T>::extractMesh(aiMesh *mesh) {
        auto* vertices = new ModelVertex[mesh->mNumVertices];
        std::vector<uint32_t> indicesVector;
        // extract vertices
        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            // positions
            vertices[i].position = {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z,
            };
            // normals
            vertices[i].normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z,
            };
            // mesh may not always contain texture coords
            if (mesh->mTextureCoords[0]) {
                vertices[i].uv = {
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y
                };
            }
            // tangents
            if (mesh->mTangents) {
                vertices[i].tangent = {
                        mesh->mTangents[i].x,
                        mesh->mTangents[i].y,
                        mesh->mTangents[i].z,
                };
            }
            // bitangents
            if (mesh->mBitangents) {
                vertices[i].bitangent = {
                        mesh->mBitangents[i].x,
                        mesh->mBitangents[i].y,
                        mesh->mBitangents[i].z,
                };
            }
        }
        // extract indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            auto face = mesh->mFaces[i];
            for(uint32_t j = 0; j < face.mNumIndices; j++) {
                indicesVector.push_back(face.mIndices[j]);
            }
        }

        array<ModelVertex> vertexData = { vertices, 0, mesh->mNumVertices };
        graphics::IndexData indexData = {
                engine::core::mapTo(indicesVector),
                0,
                static_cast<uint32_t>(indicesVector.size())
        };

        return { vertexData, indexData };
    }

    template<typename T>
    Material ModelFile<T>::extractMaterial(const std::string& texturesPath, aiMesh *mesh, const aiScene* scene) {
        // check if material exist for mesh
        Material material;
//        if (mesh->mMaterialIndex >= 0) {
//            aiMaterial* modelMaterial = scene->mMaterials[mesh->mMaterialIndex];

            // todo assimp is not getting textures by texture_type correctly.
            //      for example aiTextureType_DIFFUSE returns path to base color or to AO map
//            material.albedoSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_BASE_COLOR,
//                    material.enableAlbedoMap.value
//            );
//            material.diffuseSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_DIFFUSE,
//                    material.enableDiffuseMap.value
//            );
//            material.specularSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_SPECULAR,
//                    material.enableSpecularMap.value
//            );
//            material.normalSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_NORMALS,
//                    material.enableNormalMap.value
//            );
//            material.depthSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_DISPLACEMENT,
//                    material.enableParallaxMap.value
//            );
//            material.aoSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_AMBIENT_OCCLUSION,
//                    material.enableAOMap.value
//            );
//            material.metallicSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_METALNESS,
//                    material.enableMetallicMap.value
//            );
//            material.roughnessSlot.textureId = extractTexture(
//                    texturesPath,
//                    modelMaterial,
//                    aiTextureType_DIFFUSE_ROUGHNESS,
//                    material.enableRoughnessMap.value
//            );
//        }

        return material;
    }

    template<typename T>
    u32 ModelFile<T>::extractTexture(const std::string& texturesPath,
                                     aiMaterial* material,
                                     aiTextureType textureType, bool& enableTexture) {
        aiString texture_file;
        material->Get(AI_MATKEY_TEXTURE(textureType, 0), texture_file);
//        if(auto texture = scene->GetEmbeddedTexture(texture_file.C_Str())) {
        std::string textureName = engine::filesystem::getFileNameWithExtension(texture_file.data);
        std::stringstream ss;
        ss << texturesPath << "/" << textureName;
        std::string textureFullPath = ss.str();
        u32 textureId = TextureBuffer::load(textureFullPath.c_str());
        enableTexture = textureId != invalidTextureId;
        return textureId;
    }

    template<typename T>
    void ModelFile<T>::read(const std::string &filepath, const std::string& texturesFilePath, const ModelFileListener<T> &listener, const ModelFileOptions& options) {
        ENGINE_INFO("ModelFile: read='{0}'", filepath);
        // get a copy of mesh that's already loaded from a model file
        if (exists(filepath)) {
            listener.success(modelMap.at(filepath));
            return;
        }
        // load new mesh from model file
        try {
            auto model = read(filepath, texturesFilePath, options);
            modelMap.insert({ filepath, model });
            listener.success(model);
        } catch (const file_not_found& ex) {
            ENGINE_ERR("ModelFile: Failed to read file '{0}'", filepath);
            listener.failure(ex);
        }
    }

    template<typename T>
    bool ModelFile<T>::exists(const std::string &filepath) {
        return modelMap.find(filepath) != modelMap.end();
    }

    template<typename T>
    void ModelFile<T>::clear() {
        modelMap.clear();
    }

    template<typename T>
    Model ModelFile<T>::getFromMemory(const std::string &filepath) {
        if (exists(filepath)) {
            return modelMap.at(filepath);
        }
        ENGINE_ERR("ModelFile getFromMemory: no such model in memory filepath='{0}'", filepath);
        return {};
    }

    template<typename T>
    Model ModelFile<T>::copyFromMemory(const std::string &filepath) {
        if (exists(filepath)) {
            return modelMap.at(filepath);
        }
        ENGINE_ERR("ModelFile copyFromMemory: no such model in memory filepath='{0}'", filepath);
        return {};
    }

    template<typename T>
    T mapPos(const ModelVertex& modelVertex) {
        return { modelVertex.position };
    }

    template<typename T>
    T mapPosUv(const ModelVertex& modelVertex) {
        return { modelVertex.position, modelVertex.uv };
    }

    template<typename T>
    T map(const ModelVertex& modelVertex) {
        return { modelVertex.position, modelVertex.uv, modelVertex.normal };
    }
}
