/**
 * @file material.cpp
 * @author Ian Rudnick
 * Implementations of the scatter functions for basic materials: 
 * Lambertian, Metal, Glass.
 */

#include "material.h"

#include <cmath>

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

namespace rudnick_rt {

bool BasicLambertian::scatter(const Ray & incident, const hit_record & record,
                         RGBColor & attenuation, Ray & scattered) const {
    
    Point3 target = record.point + Vec3::randomInHemisphere(record.normal);
    auto scatter_direction = target - record.point;

    // Prevent degenerate scatter direction
    if (scatter_direction.nearZero())
        scatter_direction = record.normal;

    scattered = Ray(record.point, scatter_direction);
    attenuation = albedo_;
    return true;
}

    
} // namespace rudnick_rt

