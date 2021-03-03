/**
 * @file sphere.h
 * @author Ian Rudnick
 * Hittable sphere class for a ray-traced scene.
 */

#ifndef RUDNICKRT_SPHERE_H
#define RUDNICKRT_SPHERE_H

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

namespace rudnick_rt {

class Sphere : public Hittable {
public: // Functions
    Sphere() {}
    Sphere(Point3 center, double radius, shared_ptr<Material> material)
        : center_(center), radius_(radius), material_(material) {};

    virtual bool hit(
        const Ray & ray, double tmin, double tmax, hit_record & record
    ) const override;

public: // Data members
    Point3 center_;
    double radius_;
    shared_ptr<Material> material_;

}; // class Sphere : public Hittable

} // namespace rudnick_rt

#endif // SPHERE_H
