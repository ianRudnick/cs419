#include "aa_rectangle.h"

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {
//-----------------------------------------------------------------------------
// X - Y (wall)
bool XYRect::hit(const Ray& ray, double tmin, double tmax, hit_record& record) const {
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

bool XYRect::boundingBox(AABB& box) const {
    // Add padding so the box has nonzero width
    box = AABB(Point3(x0_, y0_, k_-0.0001), Point3(x1_, y1_, k_+0.0001));
    return true;
}

//-----------------------------------------------------------------------------
// Y - Z (wall)
bool YZRect::hit(const Ray& ray, double tmin, double tmax, hit_record& record) const {
    auto t = (k_-ray.origin().x()) / ray.direction().x();
    if (t < tmin || t > tmax)
        return false;
    auto y = ray.origin().y() + t*ray.direction().y();
    auto z = ray.origin().z() + t*ray.direction().z();
    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
        return false;

    record.u = (y-y0_) / (y1_-y0_);
    record.v = (z-z0_) / (z1_-z0_);
    record.t = t;
    auto outward_normal = Vec3(0, 0, 1);
    record.setNormalDirection(ray, outward_normal);
    record.material = m_;
    record.point = ray.at(t);
    return true;
}

bool YZRect::boundingBox(AABB& box) const {
    // Add padding so the box has nonzero width
    box = AABB(Point3(k_-0.0001, y0_, z0_), Point3(k_+0.0001, y0_, z0_));
    return true;
}

//-----------------------------------------------------------------------------
// X - Z (ground/ceiling)
bool XZRect::hit(const Ray& ray, double tmin, double tmax, hit_record& record) const {
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

bool XZRect::boundingBox(AABB & box) const {
    // Add padding so the box has nonzero width
    box = AABB(Point3(x0_, k_-0.0001, z0_), Point3(x1_, k_+0.0001, z1_));
    return true;
}
    
} // namespace rudnick_rt
