/**
 * @file aa_rectangle.h
 * @author Ian Rudnick
 * Hittable axis-alligned rectangle class for a ray-traced scene.
 * Can be moved/rotated from the axis using instancing.
 */
#ifndef RUDNICKRT_AA_RECTANGLE_H
#define RUDNICKRT_AA_RECTANGLE_H

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "utils.h"

namespace rudnick_rt {

class XYRect : public Hittable {
public:
    /**
     * Default constructor.
     * Does nothing - don't use this
     */
    XYRect() {}

    /**
     * Constructs an XYRect with the given parameters.
     * @param x0 The x-coordinate of the first corner.
     * @param x1 The x-coordinate of the second corner.
     * @param y0 The y-coordinate of the first corner.
     * @param y1 The y-coordinate of the second corner.
     * @param k The z-plane the rectangle is on.
     * @param mat The material the rectangle is made of.
     */
    XYRect(double x0, double x1, double y0, double y1, double k,
           shared_ptr<Material> mat)
           : m_(mat), x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k)  {};

    virtual bool hit(const Ray& ray, double tmin, double tmax, 
                     hit_record& record) const override;

    virtual bool boundingBox(AABB& output) const override;

private:
    shared_ptr<Material> m_;
    double x0_, x1_, y0_, y1_, k_;
};


class YZRect : public Hittable {
public:
    /**
     * Default constructor.
     * Does nothing - don't use this
     */
    YZRect() {}

    /**
     * Constructs an YZRect with the given parameters.
     * @param y0 The x-coordinate of the first corner.
     * @param y1 The x-coordinate of the second corner.
     * @param z0 The y-coordinate of the first corner.
     * @param z1 The y-coordinate of the second corner.
     * @param k The z-plane the rectangle is on.
     * @param mat The material the rectangle is made of.
     */
    YZRect(double y0, double y1, double z0, double z1, double k,
           shared_ptr<Material> mat)
           : m_(mat), y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k)  {};

    virtual bool hit(const Ray& ray, double tmin, double tmax, 
                     hit_record& record) const override;

    virtual bool boundingBox(AABB& output) const override;

private:
    shared_ptr<Material> m_;
    double y0_, y1_, z0_, z1_, k_;
};


class XZRect : public Hittable {
public:
    XZRect() {}

    /**
     * Constructs an XZRect with the given parameters.
     * @param x0 The x-coordinate of the first corner.
     * @param x1 The x-coordinate of the second corner.
     * @param z0 The z-coordinate of the first corner.
     * @param z1 The z-coordinate of the second corner.
     * @param k The y-plane the rectangle is on.
     * @param mat The material the rectangle is made of.
     */
    XZRect(double x0, double x1, double z0, double z1, double k,
           shared_ptr<Material> mat)
           : m_(mat), x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k)  {};

    virtual bool hit(const Ray& ray, double tmin, double tmax, hit_record& record) const override;

    virtual bool boundingBox(AABB& output) const override;

private:
    shared_ptr<Material> m_;
    double x0_, x1_, z0_, z1_, k_;
};
    
} // namespace rudnick_rt

#endif
