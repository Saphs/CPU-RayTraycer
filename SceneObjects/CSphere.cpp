//
// Created by Tizian Rettig on 09.12.2020.
//

#include "CSphere.h"

CSphere::CSphere() {
    m_pos = Vec();
    m_radius = 0.0f;
    m_color = Color();
}

CSphere::CSphere(Vec pos, float radius, Color color, Refl_t refl) {
    m_pos = pos;
    m_radius = radius;
    m_color = color;
    m_refl = refl;
}

bool CSphere::isIntersecting(CLine line) {
    Vec o_s_dist = sub(line.getP1(), m_pos);
    double delta = pow(dot(line.getDirection(), o_s_dist), 2) - (pow(length(o_s_dist), 2) - pow(m_radius, 2));
    return delta >= 0;
}

/**
 * Returns two intersection points between a sphere and a line. The following cases exist:
 *  - No intersection:      returns uninitialized pair. TODO: This should return a nullptr or throw an exception.
 *  - One intersection:     returns pair where both values are the same Vec.
 *  - Two intersections:    returns both intersection pints as Vec where the first value holds -b + delta and the
 *                          value holds -b - delta.
 */
Vec CSphere::firstIntersectionPoint(CLine line) {
    // See https://drive.google.com/file/d/0B8g97JkuSSBwUENiWTJXeGtTOHFmSm51UC01YWtCZw/view Page 27
    const Vec o_c = m_pos - line.getP1();
    const double b = dot(o_c, line.getDirection());
    const double det = b * b - dot(o_c, o_c) + m_radius * m_radius;

    if ( det < 0 ) {
        return Vec();
    } else {
        const double sqrt_det = sqrt(det);
        double t;

        if ( (t = b - sqrt_det) > EPSILON ) {
            return line.pointAt(t);
        } else if ( (t = b + sqrt_det) > EPSILON) {
            return line.pointAt(t);
        } else {
            return Vec();
        }
    }
}

Vec CSphere::getNormalAt(const Vec& intersection_point) {
    Vec sphere_normal = normalize(sub(intersection_point, m_pos));
    return sphere_normal;
}

Color CSphere::getMatColor() {
    return m_color;
}


