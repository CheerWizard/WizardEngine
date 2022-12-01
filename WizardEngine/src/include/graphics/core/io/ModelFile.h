//
// Created by mecha on 29.09.2021.
//

#pragma once

#include <core/filesystem.h>
#include <core/Mappers.h>
#include <graphics/core/geometry/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <sstream>

namespace engine::io {

    using namespace math;

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

    struct ENGINE_API ModelTexture {
        uint32_t id;
        aiTextureType type;
        std::string path;
    };

    struct ENGINE_API ModelMaterial {
        std::vector<ModelTexture> textures;
    };

    typedef graphics::BaseMesh<ModelVertex> ModelMesh;

    struct ENGINE_API Model {
        std::vector<ModelMesh> meshes;
        std::vector<ModelMaterial> materials;
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
        triangulate = aiProcess_Triangulate,
        flip_uv = aiProcess_FlipUVs,
        calc_tang_space = aiProcess_CalcTangentSpace,
        texture_invert = aiTextureFlags_Invert,
        embed_textures = aiProcess_EmbedTextures
    };

    struct ENGINE_API ModelFileOptions {
        vector<ModelFileOption> flags = { embed_textures, calc_tang_space, triangulate };

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
        static void read(const std::string &filepath, const ModelFileListener<T>& listener, const ModelFileOptions& options = ModelFileOptions());

    private:
        static Model read(const std::string &filePath, const ModelFileOptions& options);
        static bool exists(const std::string &filepath);
        static void clear();
        static void extractNodes(
                aiNode *node,
                const aiScene *scene,
                std::vector<ModelMesh>& meshes,
                std::vector<ModelMaterial>& materials
        );
        static ModelMesh extractMesh(aiMesh *mesh);
        static ModelMaterial extractMaterial(aiMesh *mesh, const aiScene *scene);
        static void extractTextures(std::vector<ModelTexture>& textures, aiMaterial *mat, const aiTextureType& type);
        static Model getFromMemory(const std::string& filepath);
        static Model copyFromMemory(const std::string& filepath);

    private:
        static ModelMap modelMap;
    };

    template<typename T>
    std::unordered_map<std::string, Model> ModelFile<T>::modelMap;

    template<typename T>
    Model ModelFile<T>::read(const std::string &filePath, const ModelFileOptions& options) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(filePath, options.getFlag());

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::string message = "Assimp: failed to import file '" + filePath + "'; error: " + import.GetErrorString();
            ENGINE_THROW(file_not_found(exception_priority::ERR, message));
        }

        std::vector<ModelMesh> meshes;
        std::vector<ModelMaterial> materials;
        extractNodes(scene->mRootNode, scene, meshes, materials);

        return { meshes, materials };
    }

    template<typename T>
    void ModelFile<T>::extractNodes(
            aiNode *node,
            const aiScene *scene,
            std::vector<ModelMesh>& meshes,
            std::vector<ModelMaterial>& materials
    ) {
        // extract all meshes and materials of node
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(extractMesh(mesh));
            materials.push_back(extractMaterial(mesh, scene));
        }
        // recursively extract node's children
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            extractNodes(node->mChildren[i], scene, meshes, materials);
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
    ModelMaterial ModelFile<T>::extractMaterial(aiMesh *mesh, const aiScene* scene) {
        std::vector<ModelTexture> textures;
        // check if material exist for mesh
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            extractTextures(textures, material, aiTextureType_DIFFUSE);
            extractTextures(textures, material, aiTextureType_SPECULAR);
            extractTextures(textures, material, aiTextureType_SPECULAR);
        }
        return { textures };
    }

    template<typename T>
    void ModelFile<T>::extractTextures(
            std::vector<ModelTexture>& textures,
            aiMaterial *mat,
            const aiTextureType& type
    ) {
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString filePath;
            mat->GetTexture(type, i, &filePath);
            ModelTexture texture {
                    0,
                    type,
                    std::string(filePath.data)
            };
            textures.push_back(texture);
        }
    }

    template<typename T>
    void ModelFile<T>::read(const std::string &filepath, const ModelFileListener<T> &listener, const ModelFileOptions& options) {
        ENGINE_INFO("ModelFile: read='{0}'", filepath);
        // get a copy of mesh that's already loaded from a model file
        if (exists(filepath)) {
            listener.success(modelMap.at(filepath));
            return;
        }
        // load new mesh from model file
        try {
            auto model = read(filepath, options);
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
