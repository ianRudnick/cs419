/**
 * @file hittable.h
 * @author Ian Rudnick
 * Abstract class for a hittable surface in a ray-traced scene.
 */

#ifndef RUDNICKRT_HITTABLE_H
#define RUDNICKRT_HITTABLE_H

#include "material.h"
#include "ray.h"
#include "vec3.h"

namespace rudnick_rt {

// forward declare?
class Material;

struct hit_record {
    Point3 point;
    Vec3 normal;
    shared_ptr<Material> material;
    double t;
    double u;
    double v;
    bool hit_front_of_surface;

    inline void setNormalDirection(const Ray &ray, const Vec3 &surface_normal){
        hit_front_of_surface = (Vec3::dot(ray.direction(), surface_normal) < 0);
        // If it didn't hit front of surface, reverse the normal
        normal = hit_front_of_surface ? surface_normal : -surface_normal;
    }
};

class Hittable {
public:
    /**
     * Checks whether a given ray hits the hittable object.
     * @param ray The ray to check for hit.
     * @param tmin The minimum distance to register a hit
     * @param tmax The maximum distance to register a hit
     * @param record hit_record to log the details of the hit
     * @return true if there was a hit
     */
    virtual bool hit(
        const Ray &ray, double tmin, double tmax, hit_record &record
    ) const = 0;

};

} // namespace rudnick_rt

#endif // RUDNICKRT_HITTABLE_H
