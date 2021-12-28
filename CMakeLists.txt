cmake_minimum_required(VERSION 3.16)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
add_executable(RayTracer main.cpp Vec.hpp SceneObjects/DiskLight.h SceneObjects/DiskLight.cpp SceneObjects/CTriangle.h SceneObjects/CTriangle.cpp lib/bitmap_image.hpp CLine.hpp SceneObjects/CSphere.cpp SceneObjects/CSphere.h Ray.cpp Ray.h SceneObjects/Mesh.cpp SceneObjects/Mesh.h SceneObjects/Geometry.h Color.cpp Color.h SceneObjects/Disk.cpp SceneObjects/Disk.h RayTracerUtility.hpp)