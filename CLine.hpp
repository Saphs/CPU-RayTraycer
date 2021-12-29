//
// Created by Tizian Rettig on 09.12.2020.
//

#ifndef RAYTRACER_CLINE_HPP
#define RAYTRACER_CLINE_HPP


#include "Vec.hpp"

/**
 * Simple line class representing a line with the vector equation: l = OA + t * AB where:
 *   -  OA is the vector from the origin to point a
 *   -  AB is the vector from a to b
 *   -  t is any scalar
 * @see https://en.wikipedia.org/wiki/Line_(geometry)
 */
class CLine {
public:

    CLine() {
        m_p1 = Vec();
        m_direction = normalize(Vec());
    }

    CLine(Vec p, Vec dir) {
        m_p1 = p;
        m_direction = normalize(dir);
    }

    [[nodiscard]] Vec pointAt(double t) const {
        return add(m_p1, scalarMultiply(m_direction, t));
    }

    [[nodiscard]] Vec const &getP1() const {
        return m_p1;
    }

    [[nodiscard]] Vec const &getDirection() const {
        return m_direction;
    }


private:
    Vec m_p1; // OA
    Vec m_direction; // AB
};


#endif //RAYTRACER_CLINE_HPP
