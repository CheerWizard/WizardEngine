//
// Created by mecha on 29.09.2021.
//

#include "ObjFile.h"

namespace engine {

    const char *ObjFile::getExtensionName() const {
        return ".obj";
    }

    const char *ObjFile::getAssetPath() const {
        return OBJ_PATH;
    }

    ObjData ObjFile::readObj(const std::string &fileName) {
        setAssetName(fileName);
        auto source = read();
        auto tokens = split(source, "\n\r ");

        std::vector<Vertex> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;
        std::vector<Face> faces;

        for (auto i = 0 ; i < tokens.size(); i++) {
            auto token = tokens[i];

            if (token == "v") {
                glm::vec3 position = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2]),
                        TO_FLOAT(tokens[i + 3])
                };
                vertices.emplace_back(Vertex { position });

            } else if (token == "vt") {
                glm::vec2 uv = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2])
                };
                uvs.emplace_back(uv);

            } else if (token == "vn") {
                glm::vec3 normal = {
                        TO_FLOAT(tokens[i + 1]),
                        TO_FLOAT(tokens[i + 2]),
                        TO_FLOAT(tokens[i + 3])
                };
                normals.emplace_back(normal);

            } else if (token == "f") {
                auto vertexOrder1Token = tokens[i + 1];
                auto vertexOrder2Token = tokens[i + 2];
                auto vertexOrder3Token = tokens[i + 3];

                auto faceTokens1 = split(vertexOrder1Token, "/");
                auto faceTokens2 = split(vertexOrder2Token, "/");
                auto faceTokens3 = split(vertexOrder3Token, "/");

                auto indexPos1 = TO_INT(faceTokens1[0]) - 1;
                auto indexUv1 = TO_INT(faceTokens1[1]) - 1;
                auto indexNormal1 = TO_INT(faceTokens1[2]) - 1;
                Face face1 = {
                        indexPos1,
                        indexUv1,
                        indexNormal1
                };

                auto indexPos2 = TO_INT(faceTokens2[0]) - 1;
                auto indexUv2 = TO_INT(faceTokens2[1]) - 1;
                auto indexNormal2 = TO_INT(faceTokens2[2]) - 1;
                Face face2 = {
                        indexPos2,
                        indexUv2,
                        indexNormal2
                };

                auto indexPos3 = TO_INT(faceTokens3[0]) - 1;
                auto indexUv3 = TO_INT(faceTokens3[1]) - 1;
                auto indexNormal3 = TO_INT(faceTokens3[2]) - 1;
                Face face3 = {
                        indexPos3,
                        indexUv3,
                        indexNormal3
                };

                indices.emplace_back(indexPos1);
                indices.emplace_back(indexPos2);
                indices.emplace_back(indexPos3);

                faces.emplace_back(face1);
                faces.emplace_back(face2);
                faces.emplace_back(face3);
            }
        }

        for (const auto& face : faces) {
            auto posIndex = face.posIndex;
            auto uvIndex = face.uvIndex;
            auto& vertex = vertices[posIndex];
            vertex.textureCoords = uvs[uvIndex];
        }

        uint32_t indexCount = indices.size();
        uint32_t vertexCount = vertices.size();
        auto* indicesArr = new uint32_t[indexCount];
        auto* verticesArr = new Vertex[vertexCount];

        for (auto i = 0 ; i < indexCount ; i++) {
            indicesArr[i] = indices[i];
        }

        for (auto i = 0 ; i < vertexCount ; i++) {
            verticesArr[i] = vertices[i];
        }

        auto vertexData = VertexData {
            verticesArr,
            0,
            vertexCount
        };

        auto indexData = IndexData {
            indicesArr,
            0,
            indexCount
        };

        return ObjData {
            vertexData,
            indexData
        };
    }

    void ObjFile::destroy() {
    }

}