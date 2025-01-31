//
// Created by Tizian Rettig on 13.01.2021.
//

#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H


#include <cstddef>
#include <string>

class Color {
public:
    Color();
    Color(double r, double g, double b);

    void set(double r, double g, double b);
    void set(const Color& c);

    Color operator+(const Color& c) const;
    Color operator*(const float& x) const;
    Color operator*(const double& x) const;
    [[nodiscard]] Color mult(const Color& x) const;

    [[nodiscard]] double getDoubleR() const;
    [[nodiscard]] double getDoubleG() const;
    [[nodiscard]] double getDoubleB() const;

    [[nodiscard]] std::byte getR() const;
    [[nodiscard]] std::byte getG() const;
    [[nodiscard]] std::byte getB() const;

    [[nodiscard]] std::string toString() const;

private:
    double m_r, m_g, m_b;

};


#endif //RAYTRACER_COLOR_H
