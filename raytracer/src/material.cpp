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


bool BasicMetal::scatter(const Ray & incident, const hit_record & record,
                         RGBColor & attenuation, Ray & scattered) const {
    
    Vec3 reflect_direction=Vec3::reflect(Vec3::normalize(incident.direction()),
                                         record.normal);
                                         
    Vec3 fuzz = fuzziness_ * Vec3::randomInUnitSphere();
    scattered = Ray(record.point, reflect_direction + fuzz);
    attenuation = albedo_;
    return (Vec3::dot(scattered.direction(), record.normal) > 0);
}


bool BasicDielectric::scatter(const Ray & incident, const hit_record & record,
                         RGBColor & attenuation, Ray & scattered) const {

    double refraction_ratio = record.hit_front_of_surface ? (1.0 / ri_) : ri_;

    Vec3 in_normal = Vec3::normalize(incident.direction());
    double cos_theta = fmin(Vec3::dot(-in_normal, record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    Vec3 out_direction;
    //bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    if ( (refraction_ratio * sin_theta > 1.0) ||
         (reflectance(cos_theta, refraction_ratio) > randomDouble())
       ) {

        out_direction = Vec3::reflect(in_normal, record.normal);
    }
    else {
        out_direction = Vec3::refract(in_normal, record.normal,
                                      refraction_ratio);
    }

    scattered = Ray(record.point, out_direction);
    attenuation = RGBColor(1,1,1);
    return true;
}


double BasicDielectric::reflectance(double cos, double ri) {
    auto r0 = (1 - ri) / (1 + ri);
    r0 = r0*r0;
    return r0 + (1 - r0) * pow((1 - cos), 5);
}
    
} // namespace rudnick_rt

