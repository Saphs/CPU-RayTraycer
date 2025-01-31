//
// Created by Tizian Rettig on 09.12.2020.
//

#include "CSphere.h"

CSphere::CSphere() {
    m_pos = Vec();
    m_radius2 = 0.0f;
    m_color = Color();
}

CSphere::CSphere(Vec pos, float radius, Color color, Refl_t refl) {
    m_pos = pos;
    m_radius2 = radius * radius;
    m_color = color;
    m_refl = refl;
}

bool CSphere::isIntersectingAt(CLine &line, Vec *firstIntersection) {
    Vec o_c = m_pos - line.getP1() ;
    const double b = dot(o_c, line.getDirection());
    const double det = b * b - dot(o_c, o_c) + m_radius2;

   if ( det >=  0) {
        const double sqrt_det = sqrt(det);
        double t = b - sqrt_det;

        if ( t > EPSILON ) {
            *firstIntersection =  line.pointAt(t);
            return true;
        }
        t = b + sqrt_det;
        if ( t > EPSILON) {
            *firstIntersection =  line.pointAt(t);
            return true;
        }
    }
    return false;
}

Vec CSphere::getNormalAt(const Vec& intersection_point) {
    Vec sphere_normal = normalize(intersection_point - m_pos);
    return sphere_normal;
}

Color CSphere::getMatColor() {
    return m_color;
}


