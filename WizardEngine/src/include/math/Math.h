//
// Created by mecha on 20.03.2022.
//

#pragma once

#include <core/primitives.h>
#include <io/Logger.h>
#include <functional>

namespace engine::math {

    using namespace core;

    void random(
            const s32& minRange,
            const s32& maxRange,
            const u32& count,
            const std::function<void(const u32& i, const f32& random)>& callback
    );
    f32 random(const double& minRange, const double& maxRange);
    f32 random(const s32& minRange, const s32& maxRange);

    template<typename Result, typename V1, typename V2>
    Result dot(const V1& v1, const V2& v2, u32 size) {
        Result result = 0;
        for (u32 i = 0 ; i < size ; i++) {
            result += v1[i] * v2[i];
        }
        return result;
    }

    template<typename T>
    T radians(T degrees) {
        return degrees * static_cast<T>(0.01745329251994329576923690768489);
    }

    template<typename T>
    struct vec2 {
        T v[2] = { 1, 1 };

        vec2() = default;

        vec2(T x, T y) {
            v[0] = x;
            v[1] = y;
        }

        vec2(const vec2<T>& vec) = default;

        inline T& operator [](u32 i) { return v[i]; }
        inline T operator [](u32 i) const { return v[i]; }

        void operator *=(T scalar) {
            v[0] *= scalar;
            v[1] *= scalar;
        }

        void operator +=(T scalar) {
            v[0] += scalar;
            v[1] += scalar;
        }

        void operator -=(T scalar) {
            v[0] -= scalar;
            v[1] -= scalar;
        }

        void operator /=(T scalar) {
            v[0] /= scalar;
            v[1] /= scalar;
        }

        void operator +=(const vec2<T>& vec) {
            v[0] += vec[0];
            v[1] += vec[1];
        }

        void operator -=(const vec2<T>& vec) {
            v[0] -= vec[0];
            v[1] -= vec[1];
        }

        void operator /=(const vec2<T>& vec) {
            v[0] /= vec[0];
            v[1] /= vec[1];
        }

        vec2<T>& operator =(const vec2<T>& vec) {
            v[0] = vec[0];
            v[1] = vec[1];
            return *this;
        }

        friend vec2<T> operator *(const vec2<T>& vec, T scalar) {
            return { vec[0] * scalar, vec[1] * scalar };
        }

        friend vec2<T> operator *(T scalar, const vec2<T>& vec) {
            return { vec[0] * scalar, vec[1] * scalar };
        }

        friend vec2<T> operator +(const vec2<T>& vec, T scalar) {
            return { vec[0] + scalar, vec[1] + scalar };
        }

        friend vec2<T> operator +(T scalar, const vec2<T>& vec) {
            return { vec[0] + scalar, vec[1] + scalar };
        }

        friend vec2<T> operator -(const vec2<T>& vec, T scalar) {
            return { vec[0] - scalar, vec[1] - scalar };
        }

        friend vec2<T> operator -(T scalar, const vec2<T>& vec) {
            return { scalar - vec[0], scalar - vec[1] };
        }

        friend vec2<T> operator /(const vec2<T>& vec, T scalar) {
            return { vec[0] / scalar, vec[1] / scalar };
        }

        friend vec2<T> operator /(T scalar, const vec2<T>& vec) {
            return { scalar / vec[0], scalar / vec[1] };
        }

        vec2<T> operator +(const vec2<T>& vec) const {
            return { v[0] + vec[0], v[1] + vec[1] };
        }

        vec2<T> operator -(const vec2<T>& vec) const {
            return { v[0] - vec[0], v[1] - vec[1] };
        }

        vec2<T> operator /(const vec2<T>& vec) const {
            return { v[0] / vec[0], v[1] / vec[1] };
        }

        T dotP(const vec2<T>& vec) {
            return dot(*this, vec, 2);
        }

        friend vec2<T> operator -(const vec2<T>& vec) {
            return { -vec[0], -vec[1] };
        }

        inline T x() const { return v[0]; }
        inline T y() const { return v[1]; }

        T length() const { return sqrt(x() * x() + y() * y()); }

        vec2<T> normalize() const {
            auto m = length();
            return { v[0] / m, v[1] / m };
        }
    };

    template<typename T>
    struct vec3 {
        T v[3] = { 1, 1, 1 };

        vec3() = default;

        vec3(T v0, T v1, T v2) {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }

        vec3(T v0, const vec2<T>& vec2) {
            v[0] = v0;
            v[1] = vec2.v[0];
            v[2] = vec2.v[1];
        }

        vec3(const vec2<T>& vec2, T v2) {
            v[0] = vec2.v[0];
            v[1] = vec2.v[1];
            v[2] = v2;
        }

        vec3(const vec3<T>& vec3) = default;

        inline T& operator [](u32 i) { return v[i]; }
        inline T operator [](u32 i) const { return v[i]; }

        void operator *=(T scalar) {
            v[0] *= scalar;
            v[1] *= scalar;
            v[2] *= scalar;
        }

        void operator +=(T scalar) {
            v[0] += scalar;
            v[1] += scalar;
            v[2] += scalar;
        }

        void operator -=(T scalar) {
            v[0] -= scalar;
            v[1] -= scalar;
            v[2] -= scalar;
        }

        void operator /=(T scalar) {
            v[0] /= scalar;
            v[1] /= scalar;
            v[2] /= scalar;
        }

        void operator +=(const vec3<T>& vec) {
            v[0] += vec[0];
            v[1] += vec[1];
            v[2] += vec[2];
        }

        void operator -=(const vec3<T>& vec) {
            v[0] -= vec[0];
            v[1] -= vec[1];
            v[2] -= vec[2];
        }

        void operator /=(const vec3<T>& vec) {
            v[0] /= vec[0];
            v[1] /= vec[1];
            v[2] /= vec[2];
        }

        vec3<T>& operator =(const vec3<T>& vec) {
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            return *this;
        }

        friend vec3<T> operator *(const vec3<T>& vec, T scalar) {
            return { vec[0] * scalar, vec[1] * scalar, vec[2] * scalar };
        }

        friend vec3<T> operator *(T scalar, const vec3<T>& vec) {
            return { vec[0] * scalar, vec[1] * scalar, vec[2] * scalar };
        }

        friend vec3<T> operator +(const vec3<T>& vec, T scalar) {
            return { vec[0] + scalar, vec[1] + scalar, vec[2] + scalar };
        }

        friend vec3<T> operator +(T scalar, const vec3<T>& vec) {
            return { vec[0] + scalar, vec[1] + scalar, vec[2] + scalar };
        }

        friend vec3<T> operator -(const vec3<T>& vec, T scalar) {
            return { vec[0] - scalar, vec[1] - scalar, vec[2] - scalar };
        }

        friend vec3<T> operator -(T scalar, const vec3<T>& vec) {
            return { scalar - vec[0], scalar - vec[1], scalar - vec[2] };
        }

        friend vec3<T> operator /(const vec3<T>& vec, T scalar) {
            return { vec[0] / scalar, vec[1] / scalar, vec[2] / scalar };
        }

        friend vec3<T> operator /(T scalar, const vec3<T>& vec) {
            return { scalar / vec[0], scalar / vec[1], scalar / vec[2] };
        }

        vec3<T> operator +(const vec3<T>& vec) const {
            return { v[0] + vec[0], v[1] + vec[1], v[2] + vec[2] };
        }

        vec3<T> operator -(const vec3<T>& vec) const {
            return { v[0] - vec[0], v[1] - vec[1], v[2] - vec[2] };
        }

        vec3<T> operator /(const vec3<T>& vec) const {
            return { v[0] / vec[0], v[1] / vec[1], v[2] / vec[2] };
        }

        T dotP(const vec3<T>& vec) {
            return dot(*this, vec, 3);
        }

        friend vec3<T> operator -(const vec3<T>& vec) {
            return { -vec[0], -vec[1], -vec[2] };
        }

        inline T x() const { return v[0]; }
        inline T y() const { return v[1]; }
        inline T z() const { return v[2]; }

        T length() const { return sqrt(x() * x() + y() * y() + z() * z()); }

        vec3<T> normalize() const {
            auto m = length();
            return { v[0] / m, v[1] / m, v[2] / m };
        }
    };

    template<typename T>
    vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
        return {
                v1.y() * v2.z() - v2.y() * v1.z(),
                v1.z() * v2.x() - v2.z() * v1.x(),
                v1.x() * v2.y() - v2.x() * v1.y()
        };
    }

    template<typename T>
    struct vec4 {
        T v[4] = { 1, 1, 1, 1 };

        vec4() = default;

        vec4(T value) {
            v[0] = value;
            v[1] = value;
            v[2] = value;
            v[3] = value;
        }

        vec4(T x, T y, T z, T w) {
            v[0] = x;
            v[1] = y;
            v[2] = z;
            v[3] = w;
        }

        vec4(const vec2<T>& vec, T z, T w) {
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = z;
            v[3] = w;
        }

        vec4(const vec3<T>& vec, T w) {
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            v[3] = w;
        }

        inline T& operator [](u32 i) { return v[i]; }
        inline T operator [](u32 i) const { return v[i]; }

        void operator *=(T scalar) {
            v[0] *= scalar;
            v[1] *= scalar;
            v[2] *= scalar;
            v[3] *= scalar;
        }

        void operator +=(T scalar) {
            v[0] += scalar;
            v[1] += scalar;
            v[2] += scalar;
            v[3] += scalar;
        }

        void operator -=(T scalar) {
            v[0] -= scalar;
            v[1] -= scalar;
            v[2] -= scalar;
            v[3] -= scalar;
        }

        void operator /=(T scalar) {
            v[0] /= scalar;
            v[1] /= scalar;
            v[2] /= scalar;
            v[3] /= scalar;
        }

        void operator +=(const vec4<T>& vec) {
            v[0] += vec[0];
            v[1] += vec[1];
            v[2] += vec[2];
            v[3] += vec[3];
        }

        void operator -=(const vec4<T>& vec) {
            v[0] -= vec[0];
            v[1] -= vec[1];
            v[2] -= vec[2];
            v[3] -= vec[3];
        }

        void operator /=(const vec4<T>& vec) {
            v[0] /= vec[0];
            v[1] /= vec[1];
            v[2] /= vec[2];
            v[3] /= vec[3];
        }

        vec4<T>& operator =(const vec4<T>& vec) {
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            v[3] = vec[3];
            return *this;
        }

        friend vec4<T> operator *(const vec4<T>& vec, T scalar) {
            return { vec[0] * scalar, vec[1] * scalar, vec[2] * scalar, vec[3] * scalar };
        }

        friend vec4<T> operator *(T scalar, const vec4<T>& vec) {
            return { vec[0] * scalar, vec[1] * scalar, vec[2] * scalar, vec[3] * scalar };
        }

        friend vec4<T> operator +(const vec4<T>& vec, T scalar) {
            return { vec[0] + scalar, vec[1] + scalar, vec[2] + scalar, vec[3] + scalar };
        }

        friend vec4<T> operator +(T scalar, const vec4<T>& vec) {
            return { vec[0] + scalar, vec[1] + scalar, vec[2] + scalar, vec[3] + scalar };
        }

        friend vec4<T> operator -(const vec4<T>& vec, T scalar) {
            return { vec[0] - scalar, vec[1] - scalar, vec[2] - scalar, vec[3] - scalar };
        }

        friend vec4<T> operator -(T scalar, const vec4<T>& vec) {
            return { scalar - vec[0], scalar - vec[1], scalar - vec[2], scalar - vec[3] };
        }

        friend vec4<T> operator /(const vec4<T>& vec, T scalar) {
            return { vec[0] / scalar, vec[1] / scalar, vec[2] / scalar, vec[3] / scalar };
        }

        friend vec4<T> operator /(T scalar, const vec4<T>& vec) {
            return { scalar / vec[0], scalar / vec[1], scalar / vec[2], scalar / vec[3] };
        }

        vec4<T> operator +(const vec4<T>& vec) const {
            return { v[0] + vec[0], v[1] + vec[1], v[2] + vec[2], v[3] + vec[3] };
        }

        vec4<T> operator -(const vec4<T>& vec) const {
            return { v[0] - vec[0], v[1] - vec[1], v[2] - vec[2], v[3] - vec[3] };
        }

        vec4<T> operator /(const vec4<T>& vec) const {
            return { v[0] / vec[0], v[1] / vec[1], v[2] / vec[2], v[3] / vec[3] };
        }

        T dotP(const vec4<T>& vec) const {
            return x() * vec.x() + y() * vec.y() + z() * vec.z() + w() * vec.w();
        }

        T dotP(const vec3<T>& vec) const {
            return x() * vec.x() + y() * vec.y() + z() * vec.z() + w();
        }

        friend vec4<T> operator -(const vec4<T>& vec) {
            return { -vec[0], -vec[1], -vec[2], -vec[3] };
        }

        inline T x() const { return v[0]; }
        inline T y() const { return v[1]; }
        inline T z() const { return v[2]; }
        inline T w() const { return v[3]; }

        T length() const { return sqrt(x() * x() + y() * y() + z() * z() + w() * w()); }

        vec4<T> normalize() const {
            auto m = length();
            return { v[0] / m, v[1] / m, v[2] / m, v[3] / m };
        }
    };

    typedef vec2<f32> vec2f;
    typedef vec2<s32> vec2i;
    typedef vec2<double> vec2d;

    typedef vec3<f32> vec3f;
    typedef vec3<s32> vec3i;
    typedef vec3<double> vec3d;

    typedef vec4<f32> vec4f;
    typedef vec4<s32> vec4i;
    typedef vec4<double> vec4d;

    template<typename T>
    T* values(vec2<T>& vec) {
        return &(vec.v[0]);
    }

    template<typename T>
    T* values(vec3<T>& vec) {
        return &(vec.v[0]);
    }

    template<typename T>
    T* values(vec4<T>& vec) {
        return &(vec.v[0]);
    }

    template<typename T>
    struct mat2 {
        vec2<T> v0 = { 1, 0 };
        vec2<T> v1 = { 0, 1 };

        mat2() = default;

        void operator *=(T scalar) {
            v0 *= scalar;
            v1 *= scalar;
        }

        void operator +=(T scalar) {
            v0 += scalar;
            v1 += scalar;
        }

        void operator -=(T scalar) {
            v0 -= scalar;
            v1 -= scalar;
        }

        void operator /=(T scalar) {
            v0 /= scalar;
            v1 /= scalar;
        }
    };

    template<typename T>
    struct mat3 {
        vec3<T> v0 = { 1, 0, 0 };
        vec3<T> v1 = { 0, 1, 0 };
        vec3<T> v2 = { 0, 0, 1 };

        mat3() = default;

        void operator *=(T scalar) {
            v0 *= scalar;
            v1 *= scalar;
            v2 *= scalar;
        }

        void operator +=(T scalar) {
            v0 += scalar;
            v1 += scalar;
            v2 += scalar;
        }

        void operator -=(T scalar) {
            v0 -= scalar;
            v1 -= scalar;
            v2 -= scalar;
        }

        void operator /=(T scalar) {
            v0 /= scalar;
            v1 /= scalar;
            v2 /= scalar;
        }
    };

    template<typename T>
    struct mat4 {
        vec4<T> v0 = { 1, 0, 0, 0 };
        vec4<T> v1 = { 0, 1, 0, 0 };
        vec4<T> v2 = { 0, 0, 1, 0 };
        vec4<T> v3 = { 0, 0, 0, 1 };

        mat4() = default;

        mat4(T value) {
            v0 = vec4(value);
            v1 = vec4(value);
            v2 = vec4(value);
            v3 = vec4(value);
        }

        void operator *=(T scalar) {
            v0 *= scalar;
            v1 *= scalar;
            v2 *= scalar;
            v3 *= scalar;
        }

        void operator +=(T scalar) {
            v0 += scalar;
            v1 += scalar;
            v2 += scalar;
            v3 += scalar;
        }

        void operator -=(T scalar) {
            v0 -= scalar;
            v1 -= scalar;
            v2 -= scalar;
            v3 -= scalar;
        }

        void operator /=(T scalar) {
            v0 /= scalar;
            v1 /= scalar;
            v2 /= scalar;
            v3 /= scalar;
        }

        vec4<T> col(u32 i) const {
            return { v0[i], v1[i], v2[i], v3[i] };
        }

        vec3<T> operator *=(const vec3<T>& vec) {
            return { v0.dotP(vec), v1.dotP(vec), v2.dotP(vec) };
        }

        vec3<T> operator *(const vec3<T>& vec) {
            return { v0.dotP(vec), v1.dotP(vec), v2.dotP(vec) };
        }

        vec4<T> operator *(const vec4<T>& vec) {
            return { v0.dotP(vec), v1.dotP(vec), v2.dotP(vec), v3.dotP(vec) };
        }

        mat4<T>& operator *=(const mat4<T>& mat) {
            v0[0] = v0.dotP(mat.col(0));
            v0[1] = v0.dotP(mat.col(1));
            v0[2] = v0.dotP(mat.col(2));
            v0[3] = v0.dotP(mat.col(3));

            v1[0] = v1.dotP(mat.col(0));
            v1[1] = v1.dotP(mat.col(1));
            v1[2] = v1.dotP(mat.col(2));
            v1[3] = v1.dotP(mat.col(3));

            v2[0] = v2.dotP(mat.col(0));
            v2[1] = v2.dotP(mat.col(1));
            v2[2] = v2.dotP(mat.col(2));
            v2[3] = v2.dotP(mat.col(3));

            v3[0] = v3.dotP(mat.col(0));
            v3[1] = v3.dotP(mat.col(1));
            v3[2] = v3.dotP(mat.col(2));
            v3[3] = v3.dotP(mat.col(3));

            return *this;
        }

        mat4<T> operator *(const mat4<T>& mat) const {
            auto SrcA0 = col(0);
            auto SrcA1 = col(1);
            auto SrcA2 = col(2);
            auto SrcA3 = col(3);

            auto SrcB0 = mat.col(0);
            auto SrcB1 = mat.col(1);
            auto SrcB2 = mat.col(2);
            auto SrcB3 = mat.col(3);

            mat4<T> result;

            auto col0 = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
            auto col1 = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
            auto col2 = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
            auto col3 = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

            result.v0[0] = col0[0];
            result.v1[0] = col0[1];
            result.v2[0] = col0[2];
            result.v3[0] = col0[3];

            result.v0[1] = col1[0];
            result.v1[1] = col1[1];
            result.v2[1] = col1[2];
            result.v3[1] = col1[3];

            result.v0[2] = col2[0];
            result.v1[2] = col2[1];
            result.v2[2] = col2[2];
            result.v3[2] = col2[3];

            result.v0[3] = col3[0];
            result.v1[3] = col3[1];
            result.v2[3] = col3[2];
            result.v3[3] = col3[3];

            return result;

//            mat4<T> result;
//
//            result.v0[0] = v0.dotP(mat.col(0));
//            result.v0[1] = v0.dotP(mat.col(1));
//            result.v0[2] = v0.dotP(mat.col(2));
//            result.v0[3] = v0.dotP(mat.col(3));
//
//            result.v1[0] = v1.dotP(mat.col(0));
//            result.v1[1] = v1.dotP(mat.col(1));
//            result.v1[2] = v1.dotP(mat.col(2));
//            result.v1[3] = v1.dotP(mat.col(3));
//
//            result.v2[0] = v2.dotP(mat.col(0));
//            result.v2[1] = v2.dotP(mat.col(1));
//            result.v2[2] = v2.dotP(mat.col(2));
//            result.v2[3] = v2.dotP(mat.col(3));
//
//            result.v3[0] = v3.dotP(mat.col(0));
//            result.v3[1] = v3.dotP(mat.col(1));
//            result.v3[2] = v3.dotP(mat.col(2));
//            result.v3[3] = v3.dotP(mat.col(3));
//
//            return result;
        }

        mat4<T>& operator =(const mat4<T>& mat) {
            v0 = mat.v0;
            v1 = mat.v1;
            v2 = mat.v2;
            v3 = mat.v3;
            return *this;
        }
    };

    typedef mat2<f32> mat2f;
    typedef mat2<s32> mat2i;
    typedef mat2<double> mat2d;

    typedef mat3<f32> mat3f;
    typedef mat3<s32> mat3i;
    typedef mat3<double> mat3d;

    typedef mat4<f32> mat4f;
    typedef mat4<s32> mat4i;
    typedef mat4<double> mat4d;

    template<typename T>
    T* values(mat2<T>& mat) {
        return &(mat.v0.v[0]);
    }

    template<typename T>
    T* values(mat3<T>& mat) {
        return &(mat.v0.v[0]);
    }

    template<typename T>
    T* values(mat4<T>& mat) {
        return &(mat.v0.v[0]);
    }

    template<typename T>
    T dot(const vec2<T>& v1, const vec2<T>& v2) {
        return dot(v1, v2, 2);
    }

    template<typename T>
    T dot(const vec3<T>& v1, const vec3<T>& v2) {
        return dot<T>(v1, v2, 3);
    }

    template<typename T>
    T dot(const vec4<T>& v1, const vec4<T>& v2) {
        return dot(v1, v2, 4);
    }

    template<typename T>
    mat4<T> translate(const mat4<T>& mat, const vec3<T>& vec) {
        mat4<T> result = mat;
        result.v0[3] = vec.x();
        result.v1[3] = vec.y();
        result.v2[3] = vec.z();
        return result;
    }

    template<typename T>
    mat4<T> rotateX(const mat4<T>& mat, f32 angle) {
        mat4<T> result = mat;
        auto sine = sin(math::radians(angle));
        auto cosine = cos(math::radians(angle));

        result.v1[1] = cosine;
        result.v1[2] = -sine;
        result.v2[1] = sine;
        result.v2[2] = cosine;

        return result;
    }

    template<typename T>
    mat4<T> rotateY(const mat4<T>& mat, f32 angle) {
        mat4<T> result = mat;
        auto sine = sin(math::radians(angle));
        auto cosine = cos(math::radians(angle));

        result.v0[0] = cosine;
        result.v0[2] = sine;
        result.v2[0] = -sine;
        result.v2[2] = cosine;

        return result;
    }

    template<typename T>
    mat4<T> rotateZ(const mat4<T>& mat, f32 angle) {
        mat4<T> result = mat;
        auto sine = sin(math::radians(angle));
        auto cosine = cos(math::radians(angle));

        result.v0[0] = cosine;
        result.v0[1] = -sine;
        result.v1[0] = sine;
        result.v1[1] = cosine;

        return result;
    }

    template<typename T>
    mat4<T> scale(const mat4<T>& mat, const vec3<T>& scalar) {
        mat4<T> result = mat;
        result.v0[0] = scalar.x();
        result.v1[1] = scalar.y();
        result.v2[2] = scalar.z();
        return result;
    }

    template<typename T>
    mat4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
        mat4<T> result;

        result.v0[0] = static_cast<T>(2) / (right - left);
        result.v1[1] = static_cast<T>(2) / (top - bottom);
        result.v2[2] = static_cast<T>(1) / (zFar - zNear);
        result.v3[0] = - (right + left) / (right - left);
        result.v3[1] = - (top + bottom) / (top - bottom);
        result.v3[2] = - zNear / (zFar - zNear);

        return result;
    }

    template<typename T>
    mat4<T> perspective(T fov, T aspect, T near, T far) {
        assert(aspect != static_cast<T>(0));
        assert(far != near);

        const T s = 1 / (tan(fov * PI / 360));

        mat4<T> result(static_cast<T>(0));
        result.v0[0] = s;
        result.v1[1] = s;
        result.v2[2] = -far / (far - near);
        result.v2[3] = - static_cast<T>(1);
        result.v3[2] = -(far * near) / (far - near);

        return result;
    }

    template<typename T>
    mat4<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) {
        vec3<T> f = (center - eye).normalize();
        vec3<T> s = cross(f, up).normalize();
        vec3<T> u = cross(s, f);
        mat4<T> result;

        result.v0[0] = s.x();
        result.v1[0] = s.y();
        result.v2[0] = s.z();
        result.v0[1] = u.x();
        result.v1[1] = u.y();
        result.v2[1] = u.z();
        result.v0[2] =-f.x();
        result.v1[2] =-f.y();
        result.v2[2] =-f.z();
        result.v3[0] =-dot(s, eye);
        result.v3[1] =-dot(u, eye);
        result.v3[2] = dot(f, eye);

        return result;
    }

    template<typename T>
    T distance(const vec2<T>& v1, const vec2<T>& v2) {
        return sqrt(
                ((v2.x() - v1.x()) * (v2.x() - v1.x())) +
                ((v2.y() - v1.y()) * (v2.y() - v1.y()))
        );
    }

    template<typename T>
    T distance(const vec3<T>& v1, const vec3<T>& v2) {
        return sqrt(
                ((v2.x() - v1.x()) * (v2.x() - v1.x())) +
                ((v2.y() - v1.y()) * (v2.y() - v1.y())) +
                ((v2.z() - v1.z()) * (v2.z() - v1.z()))
        );
    }

    template<typename T>
    T distance(const vec4<T>& v1, const vec4<T>& v2) {
        return sqrt(
                ((v2.x() - v1.x()) * (v2.x() - v1.x())) +
                ((v2.y() - v1.y()) * (v2.y() - v1.y())) +
                ((v2.z() - v1.z()) * (v2.z() - v1.z())) +
                ((v2.w() - v1.w()) * (v2.w() - v1.w()))
        );
    }
}