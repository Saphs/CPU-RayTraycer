//
// Created by Tizian Rettig on 01.12.2020.
//

#include <sstream>
#include "CTriangle.h"

CTriangle::CTriangle() {
}

CTriangle::CTriangle(const Vec& p1, const Vec& p2, const Vec& p3, const Vec& normal) {
    m_p1 = p1;
    m_p2 = p2;
    m_p3 = p3;
    m_faceNormal = normal;
}

std::string CTriangle::toString() const {
    std::stringstream ss;
    ss << m_p1.toString() << m_p2.toString() << m_p3.toString();
    return ss.str();
}

bool CTriangle::RayIntersectsTriangle(CLine line, Vec &outIntersectionPoint) {
    return RayIntersectsTriangle(line.getP1(), line.getDirection(), this, outIntersectionPoint);
}

bool CTriangle::RayIntersectsTriangle(Vec rayOrigin,
                                      Vec rayVector,
                                      CTriangle* inTriangle,
                                      Vec& outIntersectionPoint)
{
    const float EPSILON = 0.0001;
    Vec vertex0 = inTriangle->m_p1;
    Vec vertex1 = inTriangle->m_p2;
    Vec vertex2 = inTriangle->m_p3;
    Vec edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = cross(rayVector, edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = cross(s, edge1);
    v = f * dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        outIntersectionPoint = rayOrigin + scalarMultiply(rayVector, t);
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
