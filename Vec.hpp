//
// Created by Tizian Rettig on 11.03.2021.
//

#ifndef RAYTRACER_VEC_HPP
#define RAYTRACER_VEC_HPP


#include <cmath>
#include <string>

/** Simple Vector class to avoid complex libraries */
class Vec {
public:
    float m_x, m_y, m_z;

    Vec(): m_x(-100000), m_y(-100000), m_z(-100000){}
    Vec(float x, float y, float z): m_x(x), m_y(y), m_z(z){}

    ~Vec() = default;

    [[nodiscard]] std::string toString() const {
        return "(" + std::to_string(m_x) + "," + std::to_string(m_y) + "," + std::to_string(m_z) + ")";
    }
};

[[nodiscard]] inline Vec add(const Vec &a, const Vec &b) {
    return {
            a.m_x + b.m_x,
            a.m_y + b.m_y,
            a.m_z + b.m_z
    };
}

[[nodiscard]] inline Vec sub(const Vec &a, const Vec &b) {
    return {
            a.m_x - b.m_x,
            a.m_y - b.m_y,
            a.m_z - b.m_z
    };
}

[[nodiscard]] inline Vec scalarMultiply(const Vec &a, const float& x) {
    return {
            a.m_x * x,
            a.m_y * x,
            a.m_z * x
    };
}

[[nodiscard]] inline Vec scalarDivide(const Vec &a, const float& x) {
    return {
            a.m_x / x,
            a.m_y / x,
            a.m_z / x
    };
}

[[nodiscard]] inline float dot(const Vec &a, const Vec &b) {
    return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}

[[nodiscard]] inline Vec cross(const Vec &a, const Vec &b) {
    return {
            a.m_y * b.m_z - a.m_z * b.m_y,
            a.m_z * b.m_x - a.m_x * b.m_z,
            a.m_x * b.m_y - a.m_y * b.m_x
    };
}

[[nodiscard]] inline float length(const Vec &a) {
    float l2 = a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z;
    return (l2 != 1) ? sqrt(l2): 1;
}
[[nodiscard]] inline float length2(const Vec &a) {
    return a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z;
}

[[nodiscard]] inline Vec normalize(const Vec &a){
    float l = length(a);
    return (l != 1.f) ? Vec(
            a.m_x / l,
            a.m_y / l,
            a.m_z / l
    ) : a;
}

[[nodiscard]] inline Vec inverse(const Vec &a) {
    return {
            - a.m_x,
            - a.m_y,
            - a.m_z
    };
}

[[nodiscard]] inline float angleTo(const Vec &a, const Vec &b) {
    return acos(dot(a, b));
}

[[nodiscard]] inline Vec operator+ (const Vec &a, const Vec &b) {
    return add(a, b);
}

[[nodiscard]] inline Vec operator- (const Vec &a, const Vec &b) {
    return sub(a, b);
}

[[nodiscard]] inline Vec operator* (const float& a, const Vec &b) {
    return scalarMultiply(b, a);
}

[[nodiscard]] inline Vec operator* (const Vec &a, const float& b) {
    return scalarMultiply(a, b);
}

[[nodiscard]] inline Vec operator/ (const float& a, const Vec &b) {
    return scalarMultiply(b, 1 / a);
}

[[nodiscard]] inline Vec operator/ (const Vec &a, const float& b) {
    return scalarMultiply(a, 1 / b);
}



#endif //RAYTRACER_VEC_HPP
