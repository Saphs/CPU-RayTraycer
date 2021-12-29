//
// Created by Tizian Rettig on 08.01.2021.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H


#include "../Color.h"
#include "../CLine.hpp"

enum Refl_t {DIFF, SPEC, REFR};

class Geometry {
protected:
    constexpr static float EPSILON = 0.001;
public:
    virtual bool isIntersecting(CLine &line) = 0;
    virtual Vec firstIntersectionPoint(CLine &line) = 0;
    virtual Vec getNormalAt(const Vec& intersection_point) = 0;
    virtual Color getMatColor() = 0;

    Refl_t m_refl = DIFF;
    Color m_emissive = Color(0, 0, 0);
    float k = 0.65f;
    float shininess = 20;
};

#endif //RAYTRACER_GEOMETRY_H
