//
// Created by Tizian Rettig on 09.12.2020.
//

#ifndef RAYTRACER_CSPHERE_H
#define RAYTRACER_CSPHERE_H


#include "../Vec.hpp"
#include "Geometry.h"

class CSphere : public Geometry {
    float m_radius2;
public:
    CSphere();
    CSphere(Vec pos, float radius, Color color, Refl_t refl);

    bool isIntersectingAt(CLine &line, Vec *firstIntersection) override;
    Vec getNormalAt(const Vec& intersection_point) override;
    Color getMatColor() override;

    Vec m_pos;
    Color m_color;
};


#endif //RAYTRACER_CSPHERE_H
