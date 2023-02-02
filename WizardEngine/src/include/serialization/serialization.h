#pragma once

#include <core/array.h>
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
    file.read(reinterpret_cast<char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 2));
}

template<typename T>
void read(std::fstream& file, engine::math::vec3<T>& vec) {
    file.read(reinterpret_cast<char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 3));
}

template<typename T>
void read(std::fstream& file, engine::math::vec4<T>& vec) {
    file.read(reinterpret_cast<char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 4));
}

template<typename T>
void read(std::fstream& file, engine::math::mat2<T>& mat) {
    file.read(reinterpret_cast<char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 4));
}

template<typename T>
void read(std::fstream& file, engine::math::mat3<T>& mat) {
    file.read(reinterpret_cast<char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 9));
}

template<typename T>
void read(std::fstream& file, engine::math::mat4<T>& mat) {
    file.read(reinterpret_cast<char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 16));
}

template<>
void read<std::string>(std::fstream& file, std::string& string);

template<typename T>
void write(std::fstream& file, T& value) {
    file.write(reinterpret_cast<const char*>(&value), static_cast<std::streamsize>(sizeof(T)));
}

template<typename T>
void write(std::fstream& file, engine::core::array<T>& arr) {
    ::write(file, arr.offset);
    ::write(file, arr.size);
    ::write(file, arr.values);
}

template<typename T>
void write(std::fstream& file, engine::math::vec2<T>& vec) {
    file.write(reinterpret_cast<const char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 2));
}

template<typename T>
void write(std::fstream& file, engine::math::vec3<T>& vec) {
    file.write(reinterpret_cast<const char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 3));
}

template<typename T>
void write(std::fstream& file, engine::math::vec4<T>& vec) {
    file.write(reinterpret_cast<const char*>(&vec[0]), static_cast<std::streamsize>(sizeof(T) * 4));
}

template<typename T>
void write(std::fstream& file, engine::math::mat2<T>& mat) {
    file.write(reinterpret_cast<const char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 4));
}

template<typename T>
void write(std::fstream& file, engine::math::mat3<T>& mat) {
    file.write(reinterpret_cast<const char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 9));
}

template<typename T>
void write(std::fstream& file, engine::math::mat4<T>& mat) {
    file.write(reinterpret_cast<const char*>(&mat.v0[0]), static_cast<std::streamsize>(sizeof(T) * 16));
}

template<>
void write<std::string>(std::fstream& file, std::string& string);

#define serializable() \
ENGINE_API void serialize(YAML::Emitter& out); \
ENGINE_API void deserialize(const YAML::Node& parent); \
ENGINE_API void write(std::fstream& file); \
ENGINE_API void read(std::fstream& file);

#define serializable_unexposed() \
void serialize(YAML::Emitter& out); \
void deserialize(const YAML::Node& parent); \
void write(std::fstream& file); \
void read(std::fstream& file);