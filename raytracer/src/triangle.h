/**
 * @file triangle.h
 * @author Ian Rudnick
 * Hittable triangle class for a ray-traced scene.
 */
#ifndef RUDNICKRT_TRIANGLE_H
#define RUDNICKRT_TRIANGLE_H

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "utils.h"

namespace rudnick_rt {

class Triangle : public Hittable {
public:
	/**
	 * Default triangle constructor.
	 * Does nothing.
	 */
    Triangle() {}

	/**
	 * Constructs a triangle with the given parameters.
	 * @param v1 First vertex of the triangle
	 * @param v2 Second vertex of the triangle
	 * @param v3 Third vertex of the triangle
	 * @param mat Material the triangle is made of
	 */
    Triangle(Point3 v1, Point3 v2, Point3 v3, shared_ptr<Material> mat)
        : v1_(v1), v2_(v2), v3_(v3), m_(mat) {
		auto e1 = this->v2_ - this->v1_;
		auto e2 = this->v3_ - this->v1_;
		this->normal_ = Vec3::normalize(Vec3::cross(e1, e2));
	};

    virtual bool hit(const Ray & ray, double tmin, double tmax, 
					 hit_record & record) const override;

private:
    shared_ptr<Material> m_;
    Point3 v1_, v2_, v3_;
	Vec3 normal_;
};
    
} // namespace rudnick_rt


#endif