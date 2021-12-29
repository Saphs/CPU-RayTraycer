//
// Created by Tizian Rettig on 09.12.2020.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include <vector>
#include "CLine.hpp"
#include "SceneObjects/Geometry.h"
#include "Vec.hpp"

#define MAX_CASTING_DISTANCE 1000

/**
 *  Ray class representing one visual ray cast through the scene.
 *  @note This represents one single segment of the chain of rays that is needed for a single pixel to be evaluated.
 */
class Ray {
public:

    Ray();
    Ray(const Vec &origin, const Vec &direction);

    bool intersectWorld(Geometry** world, const int& size);
    [[nodiscard]] bool hasIntersections() const;

    [[nodiscard]] const short &getIntersectionIdx() const;


    [[nodiscard]] const CLine &getLine() const;
    [[nodiscard]] const Vec &getOrigin() const;
    [[nodiscard]] const Vec* getEnd() const;
    [[nodiscard]] const Vec &getDirection() const;
    [[nodiscard]] const float &getLength() const;

    [[nodiscard]] std::string toString() const;

private:
    bool m_hasIntersected = false;
    short m_intersection_idx = -1;
    float m_length;
    CLine m_line;
    Vec m_origin;
    Vec* m_end;
};


#endif //RAYTRACER_RAY_H
