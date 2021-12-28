//
// Created by Tiz on 19.12.2021.
//

#ifndef RAYTRACERUTILITIES_H
#define RAYTRACERUTILITIES_H

#include <chrono>
#include <random>
#include <sstream>
#include "Vec.hpp"
#include "Color.h"
#include "RayTracerUtility.hpp"

#define M_PI 3.14159265358979323846
#define OUT_DIR "../out/"

/** Define index of refraction for glass and air */
#define IOR_AIR 1
#define IOR_GLASS 1.5

/** Define some colors */
const auto RED = Color(.75f, .25f, .25f);
const auto BLUE = Color(.25f, .25f, .75f);
const auto GREEN = Color(.25f, .75f, .25f);
const auto MAGENTA = Color(.75f, .25f, .75f);
const auto CYAN = Color(.25f, .75f, .75f);
const auto YELLOW = Color(.75f, .75f, .25f);
const auto WHITE = Color(.75f, .75f, .75f);
const auto BLACK = Color(0, 0, 0);
const auto D_GREY = Color(.15f, .15f, .15f);
const auto L_GREY = Color(.5f, .5f, .5f);

/** Some typedefs and structs to clean call signatures */
struct Config {
    int buffer_width;
    int buffer_height;
    int samples_per_pixel;
    int max_iteration_depth;
};
struct ViewPlane {
    float width;
    float height;
    Vec position;
};
struct Observer {
    Vec position;
};
struct FOV {
    double alpha;
    double beta;
};

/** Prepare random number generator */
std::default_random_engine generator;
std::uniform_real_distribution<double> distr(0.0, 1.0);
double rtRandom() {
    return distr(generator);
}

/**
 * Calculates Field of View for the observer.
 * See: https://en.wikipedia.org/wiki/Field_of_view
 */
FOV calcFov(const Observer &o, const ViewPlane &vp) {
    // Assuming the center of vp and o are on the Z-axis &
    // Assuming the vp is in the XY-plane
    const float distance_to_plane = abs(o.position.m_z - vp.position.m_z);
    return {
            2 * atan( vp.width / (2 * distance_to_plane) ) * 180 / M_PI,
            2 * atan( vp.height / (2 * distance_to_plane) ) * 180 / M_PI
    };
}

/**
 * Calculates the position of the observer based on a view plane and a FOV.
 * See: https://en.wikipedia.org/wiki/Field_of_view
 */
Observer calcObserver(const float &fov_deg, const ViewPlane &vp) {
    // Assuming the center of vp and o are on the Z-axis &
    // Assuming the vp is in the XY-plane
    const double fov_rad = (fov_deg * M_PI) / 180;
    const double dist = vp.width / (2 * tan(fov_rad/2)) ;
    return {Vec(0, 0, vp.position.m_z - dist)};
}

void printContext(const Observer &o, const ViewPlane &vp, const Config &c) {
    std::cout << "Ray tracing simulation:" << std::endl;
    std::cout << " > Observer position at " << o.position.toString() << std::endl;
    std::cout << " > Viewing plane width: " << vp.width << " height: " << vp.height
              << " top left corner: "
              << vp.position.toString() << std::endl;

    double vpRatio = static_cast<double>(vp.width) / static_cast<double>(vp.height);
    double bufferRatio = static_cast<double>(c.buffer_width) / static_cast<double>(c.buffer_height);

    if (vpRatio !=  bufferRatio) {
        std::cerr << "Aspect ratio of view plane (" << vpRatio
        << ") did not match aspect ratio of image buffer (" << bufferRatio
        << ").The resulting image will be distorted." << std::endl;
    }

    const FOV fov = calcFov(o, vp);
    std::cout << " > Resulting FOV w: " << fov.alpha << "deg h: " << fov.beta << "deg" << std::endl;
    std::cout << " > Resolution: " << c.buffer_width << "x" << c.buffer_height << " pixel\n" << std::endl;
}

#ifdef __linux__
void printProgress(const std::chrono::system_clock::time_point &t_start, const int &progress, const int &img_pixel_cnt) {
    auto t_intermediate = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_intermediate - t_start).count();
    double durInS = duration / 1000;
    double relProgress = ( static_cast<double>(progress) / static_cast<double>(img_pixel_cnt) ) * 100;
    double eta = ( static_cast<double>(img_pixel_cnt) / static_cast<double>(progress) ) * durInS;
    double avgRayTime = duration / static_cast<double>(progress);

    std::cout.precision(1);
    std::cout << std::fixed << "Calculated " << relProgress << "% (" << durInS << "s)\tETA: " << eta << "s\t";
    std::cout.precision(5);
    std::cout << "avgRayTime: " << avgRayTime << "ms"<< std::endl;
}
#elif _WIN64
void printProgress(const std::chrono::time_point<std::chrono::steady_clock> &t_start, const int &progress, const int &img_pixel_cnt) {
    auto t_intermediate = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_intermediate - t_start).count();
    double durInS = duration / 1000;
    double relProgress = ( static_cast<double>(progress) / static_cast<double>(img_pixel_cnt) ) * 100;
    double eta = ( static_cast<double>(img_pixel_cnt) / static_cast<double>(progress) ) * durInS;
    double avgRayTime = duration / static_cast<double>(progress);

    std::cout.precision(1);
    std::cout << std::fixed << "Calculated " << relProgress << "% (" << durInS << "s)\tETA: " << eta << "s\t";
    std::cout.precision(5);
    std::cout << "avgRayTime: " << avgRayTime << "ms"<< std::endl;
}
#endif



void saveBmp(bitmap_image &img, const Config &c) {
    std::stringstream ss;
    ss << std::string(OUT_DIR) << "render" << c.buffer_width << "x" << c.buffer_height << "SSP" << c.samples_per_pixel << "ItD"
       << c.max_iteration_depth << ".bmp";
    img.save_image(ss.str());
    img.save_image(std::string(OUT_DIR) + "latest.bmp");

    char tmp[256];
}

#endif //RAYTRACERUTILITIES_H