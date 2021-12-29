//
// Created by Tizian Rettig on 09.12.2020.
//

#include "Ray.h"
#include <exception>
#include <sstream>

Ray::Ray() {
    m_line = CLine();
}

Ray::Ray(const Vec &origin, const Vec &direction) {

    m_line = CLine(origin, direction);
    m_end = m_line.pointAt(MAX_CASTING_DISTANCE);
    m_length = length(m_end - getOrigin());
}

std::string Ray::toString() const {
    std::stringstream ss;
   // ss << getOrigin().toString() << "->" << m_end.toString() << " m_length: " << m_length << std::endl;
    return ss.str();
}

const Vec &Ray::getOrigin() const {
    return m_line.getP1();
}

const Vec* Ray::getEnd() const {
    return &m_end;
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

const int & Ray::getIntersectionIdx() const {
    return m_intersection_idx;
}

bool Ray::intersectWorld(Geometry** world, const int& size) {
    if (!m_hasIntersected) {
        m_hasIntersected = true;
        float distance;
        Vec intersection_point;
        for ( int i = 0; i < size; i++ ) {
            if (world[i]->isIntersecting(m_line)) {
                intersection_point = world[i]->firstIntersectionPoint(m_line);
                distance = length(intersection_point -getOrigin());
                if (distance < m_length) {
                    m_end = intersection_point;
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








