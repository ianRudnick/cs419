/**
 * @file triangle.cpp
 * @author Ian Rudnick
 * Hittable triangle class implementation for a ray-traced scene.
 */
#include "triangle.h"

#include <iostream>
#include "vec3.h"

namespace rudnick_rt {

/**
 * Algorithm taken from the lecture slides
 */
bool Triangle::hit(const Ray & ray, double tmin, double tmax, hit_record & record) const {
	// Calculate determinant of matrix
	auto e1 = this->v2_ - this->v1_;
	auto e2 = this->v3_ - this->v1_;
	auto q = Vec3::cross(ray.direction(), e2);
	auto a = Vec3::dot(e1, q);
	// Test to avoid determinants close to 0
	if (a > -0.00001 && a < 0.00001) {
		return false;
	}

	auto f = 1.0 / a;
	auto s = ray.origin() - v1_;
	auto u = f * Vec3::dot(s, q);
	if (u < 0) {
		return false;
	}
	auto r = Vec3::cross(s, e1);
	auto v = f * Vec3::dot(ray.direction(), r);
	if (v < 0 || (u+v) > 1) {
		return false;
	}
	auto t = f * Vec3::dot(e2, r);
	// check if t is outside the range [tmin, tmax]
	if (t < tmin || t > tmax) {
		return false;
	}
	record.u = u;
	record.v = v;
	record.t = t;
	record.setNormalDirection(ray, this->normal_);
	record.material = this->m_;
	record.point = ray.at(t);
	return true;
}

} // namespace rudnick_rt
