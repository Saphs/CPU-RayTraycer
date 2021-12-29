//
// Created by Tizian Rettig on 05.03.2021.
//

#include "Disk.h"
#include "CSphere.h"

Disk::Disk(Vec position, Vec normal, float radius, Color c, Refl_t refl) {
    m_p0 = position;
    m_normal = normalize(normal);
    m_radius = radius;
    m_radius2 = radius * radius;
    m_color = c;
    shininess = 15;
    k = 0.8;
    m_refl = refl;
}

const Vec &Disk::getP0() const {
    return m_p0;
}

const Vec &Disk::getNormal() const {
    return m_normal;
}

bool Disk::isIntersecting(CLine &line) {
    float denom = dot(m_normal, line.getDirection());
    if (denom > EPSILON || denom < -EPSILON) {
        float t = dot( m_p0 - line.getP1() , m_normal) / denom;
        if (t >= EPSILON) {
            Vec v = line.pointAt(t) - m_p0;
            float d2 = dot(v, v);
            if (d2 <= m_radius2) {
                return true;
            }
        }
    }

    return false;
}

Vec Disk::firstIntersectionPoint(CLine &line) {
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    // assuming vectors are all normalized
    float denom = dot(m_normal, line.getDirection());
    if (denom > EPSILON || denom < -EPSILON) {
        Vec p0l0 = m_p0 - line.getP1();
        float t = dot(p0l0, m_normal) / denom;
        if (t >= EPSILON) {
            Vec p = line.pointAt(t);
            Vec v = p - m_p0;
            float d2 = dot(v, v);
            if (d2 <= m_radius2) {
                return p;
            }
        }
    }
    return Vec();
}

Vec Disk::getNormalAt(const Vec &intersection_point) {
    return m_normal;
}

Color Disk::getMatColor() {
    return m_color;
}


