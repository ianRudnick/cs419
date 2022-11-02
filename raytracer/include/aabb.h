/**
 * @file aabb.h
 * @author Ian Rudnick
 * Axis-Alligned Bounding Box class for shading/raytracing calculations.
 * Represents an xyz-axis-alligned box that fully encloses a hittable object.
 * 
 * Based on Peter Shirley's implementation in Ray Tracing: The Next Week.
 */
#ifndef RUDNICKRT_AABB_H
#define RUDNICKRT_AABB_H

#include "ray.h"
#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {

class AABB {
public:
    /**
     * Default constructor for an AABB.
     */
    AABB() {}

    /** @return The minimum corner of the AABB. */
    Point3 min() const {return min_;}

    /** @return The maximum corner of the AABB. */
    Point3 max() const {return max_;}

    /** @return The center point of the AABB. */
    Point3 centroid() const;

    /**
     * Constructs an AABB with the given parameters.
     * @param a The first corner of the box.
     * @param b The second (opposite) corner of the box.
     */
    AABB(const Point3& a, const Point3& b) {min_ = a; max_ = b;}

    /**
     * Determines whether a ray will hit the AABB.
     * @param ray The ray to check.
     * @param tmin The minimum distance along the ray to detect a hit.
     * @param tmax The maximum distance along the ray to detect a hit.
     * @return True if the ray hits the AABB between tmin and tmax.
     */
    bool hit(const Ray& ray, double tmin, double tmax) const;

    /**
     * Makes an AABB that surrounds two AABBs.
     * @param box1 The first box to surround.
     * @param box2 The second box to surround.
     * @return An AABB that surrounds both of the given boxes.
     */
    static AABB surroundingBox(const AABB& box1, const AABB& box2);

private:
    Point3 min_;
    Point3 max_;
};

} //namespace rudnick_rt

#endif