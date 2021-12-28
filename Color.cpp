//
// Created by Tizian Rettig on 13.01.2021.
//

#include <cmath>
#include <sstream>
#include "Color.h"
#define BYTE_BOUNDARY 255
#define GAMMA 2.2

Color::Color() {
    set(0, 0, 0);
}

Color::Color(double r, double g, double b) {
    set(r, g, b);
}

void Color::set(double r, double g, double b) {
    m_r = r;
    m_g = g;
    m_b = b;
}

void Color::set(const Color& c) {
    set( c.getDoubleR(), c.getDoubleG(), c.getDoubleB());
}

Color Color::operator+(const Color& c) const {
    return {
            m_r + c.getDoubleR(),
            m_g + c.getDoubleG(),
            m_b + c.getDoubleB()
            };
}

Color Color::operator*(const float& x) const {
    return {
            m_r * x,
            m_g * x,
            m_b * x
    };
}

Color Color::operator*(const double& x) const {
    return {
            m_r * x,
            m_g * x,
            m_b * x
    };
}

Color Color::mult(const Color& x) const {
    return {
            m_r * x.getDoubleR(),
            m_g * x.getDoubleG(),
            m_b * x.getDoubleB()
    };
}

inline double clamp(double x) { return x<0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x) { return int(pow(clamp(x), 1/GAMMA) * BYTE_BOUNDARY + .5); }

double Color::getDoubleR() const {
    return m_r;
}

double Color::getDoubleG() const {
    return m_g;
}

double Color::getDoubleB() const {
    return m_b;
}

std::byte Color::getR() const {
    return (std::byte) toInt(m_r);
}

std::byte Color::getG() const {
    return (std::byte) toInt(m_g);
}

std::byte Color::getB() const {
    return (std::byte) toInt(m_b);
}

std::string Color::toString() const {
    std::stringstream ss;
    ss << "d(" << getDoubleR() << ", " << getDoubleG() << ", " << getDoubleB() << ")" << "b(" << toInt(m_r) << ", " << toInt(m_g) << ", " << toInt(m_b) << ")" << std::endl;
    return ss.str();
}








