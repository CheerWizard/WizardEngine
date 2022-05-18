//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <graphics/core/io/ModelFile.h>
#include <graphics/core/geometry/Shapes.h>
#include <graphics/core/RenderModel.h>

#include "unordered_map"

namespace engine::graphics {

    template<typename T>
    struct MeshSourceListener {
        std::function<void(const BaseMeshComponent<T>)> success;
        std::function<void(const exception& exception)> failure;
        std::function<T(const io::ModelVertex&)> vertexMapper;
    };

    template<typename T>
    class MeshSource final {

        typedef std::unordered_map<std::string, BaseMeshComponent<T>> Meshes;

    private:
        MeshSource() = default;

    public:
        static void clear();
        /**
         * @return MeshComponent from either cache or .obj file.
         */
        static void getMesh(const std::string &filepath, const MeshSourceListener<T> listener);

    private:
        static bool exists(const std::string &name);

    private:
        static Meshes _meshes;
    };

    template<typename T>
    std::unordered_map<std::string, BaseMeshComponent<T>> MeshSource<T>::_meshes;

    template<typename T>
    bool MeshSource<T>::exists(const std::string &name) {
        return _meshes.find(name) != _meshes.end();
    }

    template<typename T>
    void MeshSource<T>::getMesh(const std::string &filepath, const MeshSourceListener<T> listener) {
        io::ModelFile::read(filepath, {
            [&listener](const io::Model& model) {
                auto mesh = toMeshComponent<T>(model.meshComponent, listener.vertexMapper);
                listener.success(mesh);
            },
            [&listener](const exception& exception) {
                listener.failure(exception);
            }
        });
    }

    template<typename T>
    void MeshSource<T>::clear() {
        _meshes.clear();
    }
}
