/**
 * @file hittablelist.cpp
 * @author Ian Rudnick
 * Implementation of the hittable object list.
 */

#include "hittable_list.h"

namespace rudnick_rt {

bool HittableList::hit(
    const Ray & ray, double tmin, double tmax, hit_record & record
) const {

    hit_record temp_record;
    bool hit_anything = false;
    auto closest = tmax;

    // Go through the objects vector
    for (const auto & object : objects_) {
        if (object->hit(ray, tmin, closest, temp_record)) {
            hit_anything = true;
            closest = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}

} // namespace rudnick_rt
