#include "aa_rectangle.h"

#include "vec3.h"

namespace rudnick_rt {
//-----------------------------------------------------------------------------
// X - Y (wall)
bool XYRect::hit(const Ray & ray, double tmin, double tmax, hit_record & record) const {
    auto t = (k_-ray.origin().z()) / ray.direction().z();
    if (t < tmin || t > tmax)
        return false;
    auto x = ray.origin().x() + t*ray.direction().x();
    auto y = ray.origin().y() + t*ray.direction().y();
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
        return false;

    record.u = (x-x0_) / (x1_-x0_);
    record.v = (y-y0_) / (y1_-y0_);
    record.t = t;
    auto outward_normal = Vec3(0, 0, 1);
    record.setNormalDirection(ray, outward_normal);
    record.material = m_;
    record.point = ray.at(t);
    return true;
}

//-----------------------------------------------------------------------------
// X - Z (ground/ceiling)
bool XZRect::hit(const Ray & ray, double tmin, double tmax, hit_record & record) const {
    auto t = (k_-ray.origin().y()) / ray.direction().y();
    if (t < tmin || t > tmax)
        return false;
    auto x = ray.origin().x() + t*ray.direction().x();
    auto z = ray.origin().z() + t*ray.direction().z();
    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
        return false;

    record.u = (x-x0_) / (x1_-x0_);
    record.v = (z-z0_) / (z1_-z0_);
    record.t = t;
    auto outward_normal = Vec3(0, 1, 0);
    record.setNormalDirection(ray, outward_normal);
    record.material = m_;
    record.point = ray.at(t);
    return true;
}
    
} // namespace rudnick_rt
