/**
 * @file sphere.cpp
 * @author Ian Rudnick
 * Implementation of a hittable Sphere class.
 */

#include <cmath>
#include "sphere.h"

#include "aabb.h"
#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {

bool Sphere::hit(
    const Ray & ray, double tmin, double tmax, hit_record & record
) const {
    Vec3 oc = ray.origin() - center_;
    auto a = ray.direction().lengthSquared();
    auto half_b = Vec3::dot(oc, ray.direction());
    auto c = oc.lengthSquared() - (radius_ * radius_);

    auto discriminant = (half_b * half_b) - (a * c);
    if (discriminant < 0) return false;
    auto sqrt_d = std::sqrt(discriminant);

    // Find the nearest root within [tmin, tmax]
    auto root = (-half_b - sqrt_d) / a;
    // If the first root is out of bounds, check the second root
    if (root < tmin || tmax < root) {
        root = (-half_b + sqrt_d) / a;
        // Both roots are out of bounds, so the ray does not hit
        if (root < tmin || tmax < root) return false;
    }

    record.t = root;
    record.point = ray.at(record.t);
    record.material = material_;
    Vec3 surface_normal = Vec3::normalize((record.point - center_) / radius_);
    record.setNormalDirection(ray, surface_normal);
    
    // The ray does hit
    return true;
}

bool Sphere::boundingBox(AABB& output) const {
    output = AABB(
        this->center_ - Vec3(this->radius_, this->radius_, this->radius_),
        this->center_ + Vec3(this->radius_, this->radius_, this->radius_)
    );
    return true;
}
    
} // namespace rudnick_rt

