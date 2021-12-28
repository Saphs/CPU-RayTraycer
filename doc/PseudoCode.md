## Pseudo Code
As a top-level overview on the raytrace, some pseudocode was written to illustrate its basic approach:
```
RayTracer main function {

    construct_camera_for_image_size(Width, Height)
    construct_the_scene()
    
    for ( h from 0 to Width ) {
        for ( w from 0 to Height ) {
        
            for ( i from 0 to Samples_per_pixel ) {
                r = new_ray(w, h)
                image_buffer[h][w] = image_buffer[h][w] + cast_ray(r, 0) * ( 1 / Samples_per_pixel )
            }
            
        }
    }
}
```
With the `cast_ray(ray, depth)` (actual signature `Color radiance(Ray ray, int depth)`) function being the main recursive
call and doing the heavy lifting:
```
cast_ray(ray, depth) {
    
    if (depth > max_depth):         return Black;
    if (ray.hasNoIntersections):    return Black;
    
    if (object.material == diffuse): {
        return object.emissive + object.color * cast_ray(RayAtRandomAngle(), depth + 1)
    }
    else if (object.material == specular): {
        return object.emissive + object.color * cast_ray(RayAtMirroredAngle(), depth + 1)
    }
    else if (object.material == refractive): {
        
        if (total internal reflection): {
            return object.emissive + object.color * cast_ray(RayAtMirroredAngle(), depth + 1)
        }
        else {
            R = calculateFresnelReflectance()
            reflected_radiance = cast_ray(RayAtMirroredAngle(), depth + 1) * R
            refracted_radiance = cast_ray(RayAtRefractedAngle(), depth + 1) * (1 - R)
            
            return object.emissive + object.color * (reflected_radiance + refracted_radiance)
        }
    }
    
}
```
Note: `object.emissive` describes how much light an object "puts out". Making an object with an emissive value > 0 a
light source.