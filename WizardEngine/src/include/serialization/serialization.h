#pragma once

#include <yaml/yaml.h>
#include <fstream>
#include <core/array.h>

template<typename T>
void read(std::fstream& file, T& value) {
    file.read(reinterpret_cast<char*>(&value), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void read(std::fstream& file, engine::core::array<T>& arr) {
    ::read(file, arr.offset);
    ::read(file, arr.size);
    ::read(file, arr.values);
}

template<typename T>
void read(std::fstream& file, engine::math::vec2<T>& vec) {
    file.read(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void read(std::fstream& file, engine::math::vec3<T>& vec) {
    file.read(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[2]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void read(std::fstream& file, engine::math::vec4<T>& vec) {
    file.read(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[2]), static_cast<std::streamsize>(sizeof(T)));
    file.read(reinterpret_cast<char*>(vec[3]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void read(std::fstream& file, engine::math::mat2<T>& mat) {
    ::read(file, mat.v0);
    ::read(file, mat.v1);
}

template<typename T>
void read(std::fstream& file, engine::math::mat3<T>& mat) {
    ::read(file, mat.v0);
    ::read(file, mat.v1);
    ::read(file, mat.v2);
}

template<typename T>
void read(std::fstream& file, engine::math::mat4<T>& mat) {
    ::read(file, mat.v0);
    ::read(file, mat.v1);
    ::read(file, mat.v2);
    ::read(file, mat.v3);
}

template<>
void read<std::string>(std::fstream& file, std::string& string) {
    size_t strLength;
    file.read(reinterpret_cast<char*>(&strLength), static_cast<std::streamsize>(sizeof(size_t)));
    string.resize(strLength);
    file.read(reinterpret_cast<char*>(string.data()), static_cast<std::streamsize>(sizeof(char) * strLength));
}

template<typename T>
void write(std::fstream& file, const T& value) {
    file.write(reinterpret_cast<char*>(value), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void write(std::fstream& file, const engine::core::array<T>& arr) {
    ::write(file, arr.offset);
    ::write(file, arr.size);
    ::write(file, arr.values);
}

template<typename T>
void write(std::fstream& file, const engine::math::vec2<T>& vec) {
    file.write(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void write(std::fstream& file, const engine::math::vec3<T>& vec) {
    file.write(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[2]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void write(std::fstream& file, const engine::math::vec4<T>& vec) {
    file.write(reinterpret_cast<char*>(vec[0]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[1]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[2]), static_cast<std::streamsize>(sizeof(T)));
    file.write(reinterpret_cast<char*>(vec[3]), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void write(std::fstream& file, engine::math::mat2<T>& mat) {
    ::write(file, mat.v0);
    ::write(file, mat.v1);
}

template<typename T>
void write(std::fstream& file, engine::math::mat3<T>& mat) {
    ::write(file, mat.v0);
    ::write(file, mat.v1);
    ::write(file, mat.v2);
}

template<typename T>
void write(std::fstream& file, engine::math::mat4<T>& mat) {
    ::write(file, mat.v0);
    ::write(file, mat.v1);
    ::write(file, mat.v2);
    ::write(file, mat.v3);
}

template<>
void write<std::string>(std::fstream& file, const std::string& string) {
    size_t nameSize = string.size();
    file.write(reinterpret_cast<char*>(&nameSize), static_cast<std::streamsize>(sizeof(size_t)));
    file.write(reinterpret_cast<const char*>(string.c_str()),static_cast<std::streamsize>(string.size()));
}

#define serializable() \
ENGINE_API void serialize(YAML::Emitter& out); \
ENGINE_API void deserialize(const YAML::Node& parent); \
ENGINE_API void write(std::fstream& file); \
ENGINE_API void read(std::fstream& file);

#define decl_serializable() \
void serialize(YAML::Emitter& out); \
void deserialize(const YAML::Node& parent); \
void write(std::fstream& file); \
void read(std::fstream& file);