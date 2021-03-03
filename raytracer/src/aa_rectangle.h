/**
 * @file aa_rectangle.h
 * @author Ian Rudnick
 * Hittable axis-alligned rectangle class for a ray-traced scene.
 * Can be moved/rotated from the axis using instancing.
 */
#ifndef RUDNICKRT_AA_RECTANGLE_H
#define RUDNICKRT_AA_RECTANGLE_H

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "utils.h"

namespace rudnick_rt {

class XYRect : public Hittable {
public:
    XYRect() {}

    XYRect(double x0, double x1, double y0, double y1, double k,
           shared_ptr<Material> mat)
           : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), m_(mat) {};

    virtual bool hit(const Ray & ray, double tmin, double tmax, hit_record & record) const override;

private:
    shared_ptr<Material> m_;
    double x0_, x1_, y0_, y1_, k_;
};

class XZRect : public Hittable {
public:
    XZRect() {}

    XZRect(double x0, double x1, double z0, double z1, double k,
           shared_ptr<Material> mat)
           : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), m_(mat) {};

    virtual bool hit(const Ray & ray, double tmin, double tmax, hit_record & record) const override;

private:
    shared_ptr<Material> m_;
    double x0_, x1_, z0_, z1_, k_;
};
    
} // namespace rudnick_rt

#endif
