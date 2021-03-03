/**
 * @file material.h
 * @author Ian Rudnick
 * Abstract class for materials, and some derived material classes.
 * Each material must determine what happens when an incident ray hits it.
 */

#ifndef RUDNICKRT_MATERIAL_H
#define RUDNICKRT_MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {

struct hit_record;

class Material {
public:
    /**
     * Determine what happens when an incident ray hits this material.
     * @param incident the incident ray
     * @param record a record of where the ray hit
     * @param attenuation place to store the color attenuation
     * @param scattered the scattered/bounced ray
     * @return true if a ray was scattered
     */
    virtual bool scatter(const Ray & incident,
                         const hit_record & record,
                         RGBColor & attenuation,
                         Ray & scattered) const = 0;
    
    /**
     * for emissive materials (from second Shirley book)
     */
    virtual RGBColor emitted(double u, double v, const Point3 & p) const {
        return RGBColor(0, 0, 0);
    }
};


class BasicLambertian : public Material {
public:
    BasicLambertian(const RGBColor & albedo) : albedo_(albedo) {}

    virtual bool scatter(const Ray & incident,
                         const hit_record & record,
                         RGBColor & attenuation,
                         Ray & scattered) const override;

private:
    RGBColor albedo_;
};


class RGBColorLight: public Material {
public:
    RGBColorLight(RGBColor color) : color_(color) {}

    virtual bool scatter(const Ray & incident,
                         const hit_record & record,
                         RGBColor & attenuation,
                         Ray & scattered) const override {
        return false;
    }

    virtual RGBColor emitted(double u, double v, const Point3 &p) const override {
        return color_;
    }

private:
    RGBColor color_;
};
    
} // namespace rudnick_rt

#endif
