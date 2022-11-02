/**
 * @file instance.h
 * @author Ian Rudnick
 * Object transformations using instancing.
 */

#ifndef RUDNICKRT_INSTANCE_H
#define RUDNICKRT_INSTANCE_H

#include "hittable.h"

#include "vec3.h"


namespace rudnick_rt {

/**
 * Class for translations.
 */
class Translate : public Hittable {
public:
    Translate(shared_ptr<Hittable> p, const Vec3 & displacement)
        : ptr_(p), displacement_(displacement) {}

    virtual bool hit(
        const Ray & ray, double tmin, double tmax, hit_record & record
    ) const override;

    virtual bool boundingBox(AABB & box) const override;

private:
    shared_ptr<Hittable> ptr_;
    Vec3 displacement_;
};


/**
 * Class for rotations about the y-axis.
 */
class RotateY : public Hittable {
public:
    RotateY(shared_ptr<Hittable> p, double angle);

    virtual bool hit(
        const Ray & ray, double tmin, double tmax, hit_record & record
    ) const override;

    virtual bool boundingBox(AABB & box) const override;

private:
    shared_ptr<Hittable> ptr_;
    double sin_;
    double cos_;
    bool has_box_;
    AABB box_;
};


/**
 * Class for copying an object with a different material.
 */
class Recolor : public Hittable {
public:
    Recolor(std::shared_ptr<Hittable> p, std::shared_ptr<Material> mat)
        : ptr_(p), mat_(mat) {}

    virtual bool hit(
        const Ray & ray, double tmin, double tmax, hit_record & record
    ) const override;

    virtual bool boundingBox(AABB & box) const override;

private:
    std::shared_ptr<Hittable> ptr_;
    std::shared_ptr<Material> mat_;
};
    
} // namespace rudnick_rt


#endif