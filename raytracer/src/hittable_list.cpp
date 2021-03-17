/**
 * @file hittablelist.cpp
 * @author Ian Rudnick
 * Implementation of the hittable object list.
 */

#include "hittable_list.h"

#include "aabb.h"

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

bool HittableList::boundingBox(AABB& output) const {
    if (this->objects_.empty()) {
        return false;
    }

    AABB temp_box;
    bool first_box = true;

    for (const auto& object : this->objects_) {
        if (!object->boundingBox(temp_box)) {
            return false;
        }
        output = first_box ? temp_box : AABB::surroundingBox(output, temp_box);
        first_box = false;
    }
    return true;
}

} // namespace rudnick_rt
