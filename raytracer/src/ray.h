/**
 * @file ray.h
 * @author Ian Rudnick
 * Simple ray class.
 * Uses Vec3s to represent a point and a direction for the ray.
 */

#ifndef RUDNICKRT_RAY_H
#define RUDNICKRT_RAY_H

#include "vec3.h"

namespace rudnick_rt {

class Ray {
public:
    /* Default constructor */
    Ray() {}

    /* Constructs a Ray with the given origin and direction. */
    Ray(const Point3 & origin, const Vec3 & direction)
        : origin_(origin), 
          direction_(direction) {
    }
    
    /* Returns the origin of a Ray. */
    Point3 origin() const { return origin_; }

    /* Returns the direction of a Ray. */
    Vec3 direction() const { return direction_; }

    /* Gets a point along the ray at some distance t. */
    Point3 at(double t) const {
        return origin_ + (t * direction_);
    }

public:
    Point3 origin_;
    Vec3 direction_;

}; // class Ray

} // namespace rudnick_rt

#endif // RUDNICKRT_RAY_H
