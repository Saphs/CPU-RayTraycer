//
// Created by Tizian Rettig on 05.03.2021.
//

#ifndef RAYTRACER_DISK_H
#define RAYTRACER_DISK_H


#include "../Vec.hpp"
#include "Geometry.h"

class Disk: public Geometry {
public:
    Disk() = default;
    Disk(Vec position, Vec normal, float radius, Color c, Refl_t refl);

    [[nodiscard]] const Vec &getP0() const;
    [[nodiscard]] const Vec &getNormal() const;

    bool isIntersectingAt(CLine &line, Vec *firstIntersection) override;
    Vec getNormalAt(const Vec &intersection_point) override;
    Color getMatColor() override;

private:
    Vec m_p0;
    Vec m_normal;
    float m_radius2;
    Color m_color;
};


#endif //RAYTRACER_DISK_H
