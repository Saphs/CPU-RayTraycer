//
// Created by Tizian Rettig on 08.03.2021.
//

#include "DiskLight.h"

DiskLight::DiskLight(Vec position, float radius) : CSphere(position, radius, Color(0, 0, 0), DIFF) {
    m_emissive = Color(60, 55, 50);
}
