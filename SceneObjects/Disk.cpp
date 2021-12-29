//
// Created by Tizian Rettig on 05.03.2021.
//

#include "Disk.h"
#include "CSphere.h"

Disk::Disk(Vec position, Vec normal, float radius, Color c, Refl_t refl) {
    m_p0 = position;
    m_normal = normalize(normal);
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

bool Disk::isIntersectingAt(CLine &line, Vec *firstIntersection) {
    float denom = dot(m_normal, line.getDirection());
    if (fabsf(denom) > EPSILON ) {
        float t = dot( m_p0 - line.getP1() , m_normal) / denom;
        if (t >= EPSILON) {
            *firstIntersection = line.pointAt(t);
            Vec v = *firstIntersection - m_p0;
            return  (dot(v, v) <= m_radius2);
        }
    }
    return false;
}

Vec Disk::getNormalAt(const Vec &intersection_point) {
    return m_normal;
}

Color Disk::getMatColor() {
    return m_color;
}


