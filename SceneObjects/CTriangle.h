//
// Created by Tizian Rettig on 01.12.2020.
//

#ifndef VIEWSTL_CTRIANGLE_H
#define VIEWSTL_CTRIANGLE_H

#include "../Vec.hpp"
#include "vector"
#include "../Ray.h"

/**
 * Simple Triangle class to avoid complex libraries for now.
 */
class CTriangle {
public:

    CTriangle();
    CTriangle(const Vec& p1, const Vec& p2, const Vec& p3, const Vec& normal);
    ~CTriangle(){}

    bool RayIntersectsTriangle(CLine line, Vec& outIntersectionPoint);
    std::string toString() const;

    Vec m_p1, m_p2, m_p3, m_faceNormal;

    bool pointInTriangleSurface(Vec q);
    double polygonArea2D(const std::vector<Vec>& points);

    bool
    RayIntersectsTriangle(Vec rayOrigin, Vec rayVector, CTriangle *inTriangle, Vec &outIntersectionPoint);
};


#endif //VIEWSTL_CTRIANGLE_H
