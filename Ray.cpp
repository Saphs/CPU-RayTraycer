//
// Created by Tizian Rettig on 09.12.2020.
//

#include "Ray.h"
#include <exception>
#include <sstream>

Ray::Ray() {
    m_line = CLine();
}

Ray::Ray(Vec origin, Vec direction) {

    m_line = CLine(origin, direction);
    m_origin = origin;
    auto* tmp = new Vec();
    (*tmp) = m_line.pointAt(MAX_CASTING_DISTANCE);
    m_end = tmp;
    m_length = length((*m_end) - m_origin);
    delete tmp;
}

std::string Ray::toString() const {
    std::stringstream ss;
    ss << m_origin.toString() << "->" << m_end->toString() << " m_length: " << m_length << std::endl;
    return ss.str();
}

const Vec &Ray::getOrigin() const {
    return m_origin;
}

const Vec* Ray::getEnd() const {
    return m_end;
}

const float &Ray::getLength() const {
    return m_length;
}

const Vec &Ray::getDirection() const {
    return m_line.getDirection();
}

const CLine &Ray::getLine() const {
    return m_line;
}

const short &Ray::getIntersectionIdx() const {
    return m_intersection_idx;
}

bool Ray::intersectWorld(Geometry** world, const int& size) {
    if (!m_hasIntersected) {
        m_hasIntersected = true;
        for ( short i = 0; i < (short) size; i++ ) {
            if (world[i]->isIntersecting(m_line)) {
                Vec intersection_point = world[i]->firstIntersectionPoint(m_line);
                float distance = length(intersection_point - m_origin);
                if (distance < m_length) {
                    (*m_end) = intersection_point;
                    m_length = distance;
                    m_intersection_idx = i;
                }
            }
        }
        return true;
    }
    return false;
}

bool Ray::hasIntersections() const {
    return m_hasIntersected && m_intersection_idx >= 0;
}








