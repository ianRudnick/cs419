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
	 * Constructs a triangle from three points.
	 * @param v0 First vertex of the triangle
	 * @param v1 Second vertex of the triangle
	 * @param v2 Third vertex of the triangle
	 * @param mat Material the triangle is made of
	 */
    Triangle(Point3 v0, Point3 v1, Point3 v2, shared_ptr<Material> mat)
        : v0_(v0), v1_(v1), v2_(v2), m_(mat)
	{
		auto e1 = this->v1_ - this->v0_;
		auto e2 = this->v2_ - this->v0_;
		Vec3 normal = Vec3::normalize(Vec3::cross(e1, e2));
		this->n0_ = normal;
		this->n1_ = normal;
		this->n2_ = normal;
	};

	/**
	 * Constructs a triangle with per-vertex normals.
	 * @param v0 First vertex of the triangle
	 * @param v1 Second vertex of the triangle
	 * @param v2 Third vertex of the triangle
	 * @param n0 First vertex normal vector
	 * @param n1 Second vertex normal vector
	 * @param n2 Third vertex normal vector
	 * @param mat Material the triangle is made of
	 */
	Triangle(Point3 v0, Point3 v1, Point3 v2, Vec3 n0, Vec3 n1, Vec3 n2,
			 shared_ptr<Material> mat)
		: v0_(v0), v1_(v1), v2_(v2), n0_(n0), n1_(n1), n2_(n2), m_(mat) {}

    virtual bool hit(const Ray & ray, double tmin, double tmax, 
					 hit_record & record) const override;

	virtual bool boundingBox(AABB& output) const override;

private:
    Point3 v0_, v1_, v2_;
	Vec3 n0_, n1_, n2_;
	shared_ptr<Material> m_;
};
    
} // namespace rudnick_rt

#endif
