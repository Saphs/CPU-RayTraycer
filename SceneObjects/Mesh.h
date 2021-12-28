//
// Created by Tizian Rettig on 08.01.2021.
//

#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H


#include <vector>
#include "CTriangle.h"
#include "Geometry.h"

class Mesh : public Geometry{
public:
    Mesh();
    Mesh(std::vector<CTriangle> triangles, Color color);
    Mesh(std::vector<CTriangle> triangles, Color color, std::string name);

    bool isIntersecting(CLine line) override;
    Vec firstIntersectionPoint(CLine line) override;
    Vec getNormalAt(const Vec &intersection_point) override;
    Color getMatColor() override;
    std::vector<CTriangle> m_triangles;
    Color m_color;
    std::string m_name;
};


#endif //RAYTRACER_MESH_H
