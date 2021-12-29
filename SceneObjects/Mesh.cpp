//
// Created by Tizian Rettig on 08.01.2021.
//

#include "Mesh.h"

#include <utility>
#include <iostream>

Mesh::Mesh() = default;

Mesh::Mesh(std::vector<CTriangle> triangles, Color color) {
    m_triangles = std::move(triangles);
    m_color = color;
}

Mesh::Mesh(std::vector<CTriangle> triangles, Color color, std::string name) {
    m_triangles = std::move(triangles);
    m_color = color;
    m_name = name;
}

bool Mesh::isIntersectingAt(CLine &line, Vec *firstIntersection) {
    std::vector<Vec> intersections;
    for ( auto &t : m_triangles) {
        Vec tmp;
        if (t.RayIntersectsTriangle(line, tmp)) {
           intersections.push_back(tmp);
        }
    }
    if ( intersections.empty() )
         return false;

    float minDistance = std::numeric_limits<float>::max();
    for ( auto &intersection : intersections) {
        //std::cout << ray.m_line.m_p1.toString() << std::endl;
        float distance = length2(intersection - line.getP1());
        if (distance < minDistance) {
            minDistance = distance;
             *firstIntersection = intersection;
        }
    }
    return true;
}

Vec Mesh::getNormalAt(const Vec &intersection_point) {
    Vec normal;
    for (auto &triangle : m_triangles) {
        Vec A = triangle.m_p1;
        Vec B = triangle.m_p2;
        Vec C = triangle.m_p3;
        Vec P = intersection_point;
        Vec a_c = sub(C, A);
        Vec a_b = sub(B, A);
        float a_big_triangle = (1 / 2) * length(cross(a_c, a_b));

        Vec a_p = sub(P, A);
        //CVector a_b = sub(B, A);
        float a_abp_triangle = (1 / 2) * length(cross(a_p, a_b));

        Vec b_p = sub(P, B);
        Vec b_c = sub(C, B);
        float a_bcp_triangle = (1 / 2) * length(cross(b_p, b_c));

        Vec c_p = sub(P, C);
        Vec c_a = sub(A, C);
        float a_cap_triangle = (1 / 2) * length(cross(c_p, c_a));

        if (a_abp_triangle + a_bcp_triangle + a_cap_triangle - a_big_triangle <= 0.00001) {
            normal = triangle.m_faceNormal;
        }else{
            std::cout << "Error: No triangle found" << std::endl;
        }
    }
    return normal;
}

Color Mesh::getMatColor() {
    return m_color;
}


