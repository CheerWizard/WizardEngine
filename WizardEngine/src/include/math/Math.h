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

        void operator +=(const vec2<T>& vec) {
            v[0] += vec[0];
            v[1] += vec[1];
        }

        void operator -=(const vec2<T>& vec) {
            v[0] -= vec[0];
            v[1] -= vec[1];
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

        T operator *(const vec2<T>& vec) {
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

        void dot(const vec2<T>& vec, u32 i) {
            v[i] = v[0] * vec.x() + v[1] * vec.y();
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

        vec3<T> operator +(const vec3<T>& vec) const {
            return { v[0] + vec[0], v[1] + vec[1], v[2] + vec[2] };
        }

        vec3<T> operator -(const vec3<T>& vec) const {
            return { v[0] - vec[0], v[1] - vec[1], v[2] - vec[2] };
        }

        vec3<T> operator /(const vec3<T>& vec) const {
            return { v[0] / vec[0], v[1] / vec[1], v[2] / vec[2] };
        }

        T operator *(const vec3<T>& vec) {
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

        void dot(const vec3<T>& vec, u32 i) {
            v[i] = v[0] * vec.x() + v[1] * vec.y() + v[2] * vec.z();
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

        vec4(T x, T y, T z, T w) {
            v[0] = x;
            v[1] = y;
            v[2] = z;
            v[3] = w;
        }

        vec4(const vec3<T>& vec, T v3) {
            v[0] = vec[0];
            v[1] = vec[1];
            v[2] = vec[2];
            v[3] = v3;
        }

        inline T& operator [](u32 i) { return v[i]; }
        inline T operator [](u32 i) const { return v[i]; }

        void operator *=(T scalar) {
            v[0] *= scalar;
            v[1] *= scalar;
            v[2] *= scalar;
            v[3] *= scalar;
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

        vec4<T> operator +(const vec4<T>& vec) const {
            return { v[0] + vec[0], v[1] + vec[1], v[2] + vec[2], v[3] + vec[3] };
        }

        vec4<T> operator -(const vec4<T>& vec) const {
            return { v[0] - vec[0], v[1] - vec[1], v[2] - vec[2], v[3] - vec[3] };
        }

        vec4<T> operator /(const vec4<T>& vec) const {
            return { v[0] / vec[0], v[1] / vec[1], v[2] / vec[2], v[3] / vec[3] };
        }

        T operator *(const vec4<T>& vec) {
            return dot(*this, vec, 4);
        }

        friend vec4<T> operator -(const vec4<T>& vec) {
            return { -vec[0], -vec[1], -vec[2], -vec[3] };
        }

        inline T x() const { return v[0]; }
        inline T y() const { return v[1]; }
        inline T z() const { return v[2]; }
        inline T w() const { return v[3]; }

        T length() { return sqrt(x() * x() + y() * y() + z() * z() + w() * w()); }

        vec4<T> normalize() const {
            auto m = length();
            return { v[0] / m, v[1] / m, v[2] / m, v[3] / m };
        }

        void dot(const vec4<T>& vec, u32 i) {
            v[i] = v[0] * vec.x() + v[1] * vec.y() + v[2] * vec.z() + v[3] * vec.w();
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

        void operator *(T scalar) {
            v0 * scalar;
            v1 * scalar;
        }
    };

    template<typename T>
    struct mat3 {
        vec3<T> v0 = { 1, 0, 0 };
        vec3<T> v1 = { 0, 1, 0 };
        vec3<T> v2 = { 0, 0, 1 };

        mat3() = default;

        void operator *(T scalar) {
            v0 * scalar;
            v1 * scalar;
            v2 * scalar;
        }
    };

    template<typename T>
    struct mat4 {
        vec4<T> v0 = { 1, 0, 0, 0 };
        vec4<T> v1 = { 0, 1, 0, 0 };
        vec4<T> v2 = { 0, 0, 1, 0 };
        vec4<T> v3 = { 0, 0, 0, 1 };

        mat4() = default;

        mat4<T>& operator *(T scalar) {
            v0 * scalar;
            v1 * scalar;
            v2 * scalar;
            v3 * scalar;
            return *this;
        }

        vec4<T> col(u32 i) const {
            return { v0[i], v1[i], v2[i], v3[i] };
        }

        // returns column vector
        vec4<T> operator [](u32 i) const {
            return col(i);
        }

        mat4<T>& operator *(const vec3<T>& vec) {
            v0 * vec.v[0];
            v1 * vec.v[1];
            v2 * vec.v[2];
            return *this;
        }

        mat4<T>& operator *(const vec4<T>& vec) {
            v0 * vec.v[0];
            v1 * vec.v[1];
            v2 * vec.v[2];
            v3 * vec.v[3];
            return *this;
        }

        mat4<T>& operator *(const mat4<T>& mat) {
            v0.dot(mat[0], 0);
            v0.dot(mat[1], 1);
            v0.dot(mat[2], 2);
            v0.dot(mat[3], 3);

            v1.dot(mat[0], 0);
            v1.dot(mat[1], 1);
            v1.dot(mat[2], 2);
            v1.dot(mat[3], 3);

            v2.dot(mat[0], 0);
            v2.dot(mat[1], 1);
            v2.dot(mat[2], 2);
            v2.dot(mat[3], 3);

            v3.dot(mat[0], 0);
            v3.dot(mat[1], 1);
            v3.dot(mat[2], 2);
            v3.dot(mat[3], 3);

            return *this;
        }

        mat4<T> operator *(const mat4<T>& mat) const {
            return {
                    dot(v0, mat[0]),
                    dot(v0, mat[1]),
                    dot(v0, mat[2]),
                    dot(v0, mat[3]),

                    dot(v1, mat[0]),
                    dot(v1, mat[1]),
                    dot(v1, mat[2]),
                    dot(v1, mat[3]),

                    dot(v2, mat[0]),
                    dot(v2, mat[1]),
                    dot(v2, mat[2]),
                    dot(v2, mat[3]),

                    dot(v3, mat[0]),
                    dot(v3, mat[1]),
                    dot(v3, mat[2]),
                    dot(v3, mat[3]),
            };
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
        result.v3 = mat.v0 * vec.v[0] + mat.v1 * vec[1] + mat.v2 * vec.v[2] + mat.v3;
        return result;
    }

    template<typename T>
    mat4<T> rotate(const mat4<T>& mat, f32 angle, const vec3<T>& v) {
        const T a = angle;
        const T c = cos(a);
        const T s = sin(a);

        vec3<T> axis(v.normalize());
        vec3<T> temp((T(1) - c) * axis);

        mat4<T> rotMat;
        rotMat.v0[0] = c + temp[0] * axis[0];
        rotMat.v0[1] = temp[0] * axis[1] + s * axis[2];
        rotMat.v0[2] = temp[0] * axis[2] - s * axis[1];

        rotMat.v1[0] = temp[1] * axis[0] - s * axis[2];
        rotMat.v1[1] = c + temp[1] * axis[1];
        rotMat.v1[2] = temp[1] * axis[2] + s * axis[0];

        rotMat.v2[0] = temp[2] * axis[0] + s * axis[1];
        rotMat.v2[1] = temp[2] * axis[1] - s * axis[0];
        rotMat.v2[2] = c + temp[2] * axis[2];

        mat4<T> result;
        result.v0 = mat.v0 * rotMat.v0[0] + mat.v1 * rotMat.v0[1] + mat.v2 * rotMat.v0[2];
        result.v1 = mat.v0 * rotMat.v1[0] + mat.v1 * rotMat.v1[1] + mat.v2 * rotMat.v1[2];
        result.v2 = mat.v0 * rotMat.v2[0] + mat.v1 * rotMat.v2[1] + mat.v2 * rotMat.v2[2];
        result.v3 = mat.v3;
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
    mat4<T> perspective(T fov, T aspect, T zNear, T zFar) {
        if (abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0)) {
            ENGINE_ERR("perspective(): assertion failed!");
        }

        T const tanHalfFov = tan(fov / static_cast<T>(2));
        mat4<T> result;

        result.v0[0] = static_cast<T>(1) / (aspect * tanHalfFov);
        result.v1[1] = static_cast<T>(1) / (tanHalfFov);
        result.v2[2] = zFar / (zFar - zNear);
        result.v2[3] = static_cast<T>(1);
        result.v3[2] = -(zFar * zNear) / (zFar - zNear);

        return result;
    }

    template<typename T>
    mat4<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up) {
        const vec3<T> f((center - eye).normalize());
        const vec3<T> s(cross(f, up).normalize());
        const vec3<T> u(cross(s, f));

        mat4<T> result;
        result.v0[0] = s[0];
        result.v1[0] = s[1];
        result.v2[0] = s[2];
        result.v0[1] = u[0];
        result.v1[1] = u[1];
        result.v2[1] = u[2];
        result.v0[2] =-f[0];
        result.v1[2] =-f[1];
        result.v2[2] =-f[2];
        result.v3[0] =-dot(s, eye);
        result.v3[1] =-dot(u, eye);
        result.v3[2] = dot(f, eye);

        return result;
    }

}