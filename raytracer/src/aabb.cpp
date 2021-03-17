/**
 * @file aabb.cpp
 * @author Ian Rudnick
 * Implementation of the Axis-Alligned Bounding Box.
 * 
 * Based on Peter Shirley's implementation in Ray Tracing: The Next Week.
 */
#include "aabb.h"
#include <algorithm>

namespace rudnick_rt {

/**
 * Using the optimized hit method proposed by Andrew Kensler at Pixar.
 */
bool AABB::hit(const Ray& ray, double tmin, double tmax) const {

    for (int i = 0; i < 3; i++) {
        auto inv_dir = 1.0f / ray.direction()[i];
        auto t0 = (min()[i] - ray.origin()[i]) * inv_dir;
        auto t1 = (max()[i] - ray.origin()[i]) * inv_dir;

        if (inv_dir < 0.0f) {
            std::swap(t0, t1);
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;

        if (tmax <= tmin) {
            return false;
        }
    }
    return true;
}

AABB AABB::surroundingBox(const AABB& box1, const AABB& box2) {
    Point3 min(fmin(box1.min().x(), box2.min().x()),
               fmin(box1.min().y(), box2.min().y()),
               fmin(box1.min().z(), box2.min().z()));

    Point3 max(fmax(box1.max().x(), box2.max().x()),
               fmax(box1.max().y(), box2.max().y()),
               fmax(box1.max().z(), box2.max().z()));

    return AABB(min, max);
}
    
} // namespace rudnick_rt
