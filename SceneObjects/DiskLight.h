//
// Created by Tizian Rettig on 08.03.2021.
//

#ifndef RAYTRACER_DISKLIGHT_H
#define RAYTRACER_DISKLIGHT_H


#include "CSphere.h"

class DiskLight: public CSphere {
public:
    DiskLight() = default;
    DiskLight(Vec position, float radius);

};


#endif //RAYTRACER_DISKLIGHT_H
