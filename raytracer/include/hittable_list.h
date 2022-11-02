/**
 * @file hittablelist.h
 * @author Ian Rudnick
 * A list of hittable objects to render together.
 */

#ifndef RUDNICKRT_HITTABLELIST_H
#define RUDNICKRT_HITTABLELIST_H

#include <memory>
#include <vector>

#include "hittable.h"
#include "ray.h"

namespace rudnick_rt {

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects_.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects_.push_back(object); }

    virtual bool hit(const Ray & ray,
                     double tmin, double tmax, 
                     hit_record & record) const override;
    
    virtual bool boundingBox(AABB& output) const override;

public:
    std::vector<std::shared_ptr<Hittable>> objects_;

}; // class HittableList

} // namespace rudnick_rt

#endif // HITTABLELIST_H
