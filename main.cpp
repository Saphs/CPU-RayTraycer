
/**
 * This (CPU) Raytracer application was written by Tizian Rettig in WS 2020/2021.
 *
 * It is written as a standalone project using only one light weight library
 * for reading and writing .bmp files. The original version of this project were
 * heavily inspired by the project: "smallpt: Global Illumination in 99 lines of C++"
 * initially written 4/29/2007 by Kevin Beason. See: "https://www.kevinbeason.com/smallpt/"
 *
 * More information is provided in the Readme.md of this project.
 */

#include <iostream>
#include "lib/bitmap_image.hpp"
#include "Vec.hpp"
#include "CLine.hpp"
#include "RayTracerUtility.hpp"
#include "SceneObjects/CSphere.h"
#include "SceneObjects/Mesh.h"
#include <cmath>
#include <chrono>
#include <array>
#include "SceneObjects/Disk.h"
#include "SceneObjects/DiskLight.h"

/** Define configurable values */
// 16:9 resolutions: 160x90, 320x180, 640x360, 1024x576, 1280x720 (HD-ready), 1920x1080 (full-HD), 2560x1440 (WQHD), 3840x2160 (4K)
#define BUFFER_WIDTH 1280
#define BUFFER_HEIGHT 720
#define MAX_ITERATION_DEPTH 10
#define SAMPLES_PER_PIXEL 5

/** Declare world geometry data structure */
#define SCENE_OBJECT_CNT 11
Geometry* geometries[SCENE_OBJECT_CNT];

/**
 * Calculate random ray for diffuse distribution on the surface point hit based on the surface normal
 * See https://drive.google.com/file/d/0B8g97JkuSSBwUENiWTJXeGtTOHFmSm51UC01YWtCZw/view page 56
 */
inline Ray diffuseRandomRay(const Vec &surface_p, const Vec &normal) {
    double r1 = 2 * M_PI * rtRandom();
    double r2 = rtRandom();
    double r2s = sqrt(r2);

    /** Construct orthonormal coordinate frame (w, u, v) based on surface normal */
    Vec w = normal;
    Vec u = normalize(cross(fabs(w.m_x) > .1 ? Vec(0, 1, 0) : Vec(1, 0, 0), w));
    Vec v = cross(w, u);

    /** Build random directional vector */
    Vec d = normalize(u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2));
    return {surface_p, d};
}

/** Mirror the origin direction on the surface point hit along the normal of the surface */
inline Ray reflectRay(const Vec &surface_p, const Vec &normal, const Vec &origin_dir) {
    return {surface_p, origin_dir - normal * 2 * dot(normal, origin_dir)};
}

/**
 * Recursive call that traces one specific ray through the scene.
 *
 * @param ray the ray that is followed through the scene
 * @param depth the current depth the ray was already followed (restricted by MAX_ITERATION_DEPTH macro)
 * @return the color of the pixel corresponding to the ray that was traced
 */
Color radiance(Ray ray, int depth) {

    /** Check depth to stop recursion */
    if (++depth > MAX_ITERATION_DEPTH) return BLACK;

    /** Calculate intersections & return if 0 were found */
    ray.intersectWorld(geometries, SCENE_OBJECT_CNT);
    if (!ray.hasIntersections()) {
        return BLACK;
    }

    /** Surface properties */
    Geometry *g = geometries[ray.getIntersectionIdx()];
    Vec inter_p = (*ray.getEnd());
    Vec ray_normal = g->getNormalAt((*ray.getEnd()));
    Vec inter_p_normal = dot(ray_normal, ray.getDirection()) < 0 ? ray_normal : inverse(ray_normal); // orient normal even for refraction
    Color obj_c = g->getMatColor();

    /** Diffuse reflection */
    if (g->m_refl == DIFF) {
        Ray d_ray = diffuseRandomRay(inter_p, inter_p_normal);
        return g->m_emissive + obj_c.mult(radiance(d_ray, depth));

    /** Specular reflection */
    } else if (g->m_refl == SPEC) {
        Ray s_ray = reflectRay(inter_p, inter_p_normal, ray.getDirection());
        return g->m_emissive + obj_c.mult(radiance(s_ray, depth));

    /** Refraction */
    } else if (g->m_refl == REFR) {
        Ray refl_ray = reflectRay(inter_p, inter_p_normal, ray.getDirection());

        // flag signaling if transition is from external to internal of the object or vice versa
        bool isExt_To_Int = dot(ray_normal, inter_p_normal) > 0;
        float n_quo = isExt_To_Int ? IOR_AIR / IOR_GLASS : IOR_GLASS / IOR_AIR;
        float cos_theta_I = dot(ray.getDirection(), inter_p_normal);
        float cos2_theta_t = 1 - n_quo * n_quo * (1 - cos_theta_I * cos_theta_I); // calculate cos²(theta_t) using Snell's law

        /** Total internal reflection */
        if (cos2_theta_t < 0) {
            return g->m_emissive + obj_c.mult(radiance(refl_ray, depth));

        /** Partial internal reflection + refraction */
        } else {
            float sign = isExt_To_Int ? 1.0f : -1.0f; // adjust for non-inverted normal as cos_theta_I is required to be positive in the original term
            Vec refract_dir = normalize(ray.getDirection() * n_quo - ray_normal * (sign * (cos_theta_I * n_quo + sqrt(cos2_theta_t))));

            /** Approximate Fresnel reflectance. "which percentage of light is refracted or reflected" */
            double a = IOR_GLASS - IOR_AIR;
            double b = IOR_GLASS + IOR_AIR;
            double FR_norm = (a * a) / (b * b); // Reflectance at normal incidence based on IOR (n_a & n_b)
            double c = 1 - (isExt_To_Int ? -cos_theta_I : dot(refract_dir, ray_normal)); // 1 - cos(theta_t) adjust for non-inverted normal
            double FR_eff = FR_norm + (1 - FR_norm) * c * c * c * c * c; // Fresnel reflectance using Schlick's approximation

            /** Translate reflectance into weights and probabilities */
            double Tr = 1 - FR_eff; // inverse of Fresnel reflectance
            double P = 0.25f + 0.5f * FR_eff; // Probability of reflection
            double RP = FR_eff / P;
            double TP = Tr / (1 - P);

            if (depth <= 2) { // If in the first to iterations scatter into 2 rays; Allow mild exponential growth
                Color rad = radiance( refl_ray, depth) * FR_eff + radiance(Ray(inter_p, refract_dir), depth) * Tr;
                return g->m_emissive + obj_c.mult(rad);
            } else {
                if (rtRandom() < P) { // Reflect or refract based on probability to avoid further exponential growth
                    Color rad = radiance(refl_ray, depth) * RP;
                    return g->m_emissive + obj_c.mult(rad);
                } else {
                    Color rad = radiance(Ray(inter_p, refract_dir), depth) * TP;
                    return g->m_emissive + obj_c.mult(rad);
                }
            }
        }

    } else {
        throw std::invalid_argument("A geometry had no reflective property enabled.");
    }
}

inline void setPixel(bitmap_image &img, const int &x, const int &y, const Color &c) {
    img.set_pixel(x, y, (unsigned char) c.getR(), (unsigned char) c.getG(), (unsigned char) c.getB());
}

int main() {
    const Config c = { BUFFER_WIDTH, BUFFER_HEIGHT, SAMPLES_PER_PIXEL, MAX_ITERATION_DEPTH };

    /** Build mathematical model that represents the 3D "camera". The scene will be projected onto the view plane. */
    const float aspect_width = 16;
    const float aspect_height = 9;
    const Vec vp_pos = Vec(-(aspect_width / 2), (aspect_height / 2), -30);
    const ViewPlane vp = { aspect_width, aspect_height, vp_pos };
    const Observer observer = calcObserver(70, vp);
    printContext(observer, vp, c);

    /** Define geometries */
    Disk disk_BW = Disk(Vec(0, 0, 10), Vec(0, 0, -1), 50, WHITE, DIFF);
    Disk disk_LW = Disk(Vec(-20, 0, 0), Vec(1, 0, 0), 50, RED, DIFF);
    Disk disk_RW = Disk(Vec(20, 0, 0), Vec(-1, 0, 0), 50, BLUE, DIFF);
    Disk disk_GW = Disk(Vec(0, -10, 0), Vec(0, 1, 0), 50, WHITE, DIFF);
    Disk disk_TW = Disk(Vec(0, 10, 0), Vec(0, -1, 0), 50, WHITE, DIFF);
    Disk disk_FW = Disk(Vec(0, 0, -31), Vec(0, 0, 1), 50, GREEN, DIFF);

    CSphere sphere_gold(Vec(-2, -6.5, 0), 3.f, YELLOW, DIFF);
    sphere_gold.k = 0.15f;
    sphere_gold.shininess = 64;

    CSphere sphere_green(Vec(-12.0, -4.0, 0), 5.5f, WHITE, SPEC);
    sphere_green.shininess = 5;

    CSphere sphere_L_GREY(Vec(13, -5, 0), 4.5f, WHITE, REFR);
    sphere_L_GREY.k = 0.2f;

    CSphere sphere_yellow(Vec(0, -0, 0), 1.f, MAGENTA, DIFF);

    DiskLight light = DiskLight(Vec(5, 69.9, -10), 60.f);

    /** Push geometries into storage vector */
   geometries[0] = &sphere_gold;
    geometries[1] = &sphere_L_GREY;
   geometries[2] = &sphere_yellow;
    geometries[3] = &sphere_green;

    geometries[4] = &light;

    geometries[5] = &disk_BW;
    geometries[6] = &disk_LW;
    geometries[7] = &disk_RW;
    geometries[8] = &disk_GW;
    geometries[9] = &disk_TW;
    geometries[10] = &disk_FW;

    /** Allocate image buffer */
    bitmap_image bmp_image(c.buffer_width, c.buffer_height);
    bmp_image.clear();

    /** Prepare timing values */
    int processed_pixel_cnt = 0, img_pixel_cnt = c.buffer_height * c.buffer_width, progress_marker = (int) (img_pixel_cnt / 20);
    std::cout << "Tracing " << img_pixel_cnt << " * " << 5 << " * " << c.samples_per_pixel << " rays over " << SCENE_OBJECT_CNT << " objects ..." << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();

    /** Allocate values that are constant for each pixel */
    const double d1_spp = 1.0 / c.samples_per_pixel;
    const float pixel_width = vp.width / (float) c.buffer_width;
    const float pixel_height = vp.height / (float) c.buffer_height;
    const float pixel_width_2 = pixel_width / 2;
    const float pixel_height_2 = pixel_height / 2;
    const float sub_off_w = pixel_width / 4;
    const float sub_off_h = pixel_height / 4;
    Ray r;
    Color rad, pixel;
    /** Cast h * w visual rays into the scene */
    std::array<Vec, 5> aa_sub_pixels;
    const float d1_aa_pixel_cnt = 1.0f / aa_sub_pixels.size();
    for ( int h = 0; h < c.buffer_height; h++) {
        for ( int w = 0; w < c.buffer_width; w++) {

            /** Calculate 5 sub-pixel positions that will later represent one pixel. Effectively implementing Anti-Aliasing */
            auto w_offset = static_cast<float>(static_cast<float>(w) * pixel_width) + pixel_width_2;
            auto h_offset = static_cast<float>(static_cast<float>(-h) * pixel_height) - pixel_height_2;
            aa_sub_pixels[0] = Vec(w_offset, h_offset, 0) + vp.position;
            aa_sub_pixels[1] = aa_sub_pixels[0] + Vec(-sub_off_w,  sub_off_h, 0);
            aa_sub_pixels[2] = aa_sub_pixels[0] + Vec( sub_off_w,  sub_off_h, 0);
            aa_sub_pixels[3] = aa_sub_pixels[0] + Vec(-sub_off_w, -sub_off_h, 0);
            aa_sub_pixels[4] = aa_sub_pixels[0] + Vec( sub_off_w, -sub_off_h, 0);

            /** Accumulate pixel, with n samples per pixel into one correctly lit pixel */
            pixel = BLACK;
            for (int j = 0; j < c.samples_per_pixel; j++) {
                /** Accumulate 5 sub-pixel samples into one pixel measurement (Anti-Aliasing) */
                rad = BLACK;
                for (auto v : aa_sub_pixels) {
                    r = Ray(v, v - observer.position);
                    rad = rad + radiance(r, 0) * d1_aa_pixel_cnt;
                }
                pixel = pixel + rad * d1_spp;
            }
            setPixel(bmp_image, w, h, pixel);

            /** Print progression every so often (after each 5% of the image) */
            processed_pixel_cnt++;
            if (processed_pixel_cnt % progress_marker == 0) {
                printProgress(t_start, processed_pixel_cnt, img_pixel_cnt);
            }

        }
    }

    /** Calc elapsed time & save the image */
    auto t_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
    std::cout << "Calculated 100% Done!\n" << std::endl;
    std::cout << "Time elapsed: " << duration / 1000 << "s " << duration % 1000 << "ms" << std::endl;
    saveBmp(bmp_image, c);

    return 0;
}
