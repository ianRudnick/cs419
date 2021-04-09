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
bool Triangle::hit(const Ray& ray, double tmin, double tmax,
				   hit_record& record) const
{
	auto epsilon = 0.00001;

	// Calculate determinant of matrix
	auto e1 = this->v1_ - this->v0_;
	auto e2 = this->v2_ - this->v0_;
	auto q = Vec3::cross(ray.direction(), e2);
	auto a = Vec3::dot(e1, q);
	// Check if ray is parallel to triangle. If so, it doesn't hit.
	if (a > -epsilon && a < epsilon) {
		return false;
	}

	auto f = 1.0 / a;
	auto s = ray.origin() - v0_;
	auto u = f * Vec3::dot(s, q);
	if (u < 0.0 || u > 1.0) {
		return false;
	}
	auto r = Vec3::cross(s, e1);
	auto v = f * Vec3::dot(ray.direction(), r);
	if (v < 0.0 || (u+v) > 1.0) {
		return false;
	}
	auto t = f * Vec3::dot(e2, r);
	// check if t is outside the range [tmin, tmax]
	if (t < epsilon) {
		return false;
	}

	// Record the uv coordinates of the hit, and the dist along the ray
	//record.u = u;
	//record.v = v;
	record.t = t;

	// Compute the normal vector at the hit point with barycentric interpolation
	Vec3 interpolated_normal = n0_*(1-u-v) + n1_*u + n2_*v;
	record.setNormalDirection(ray, interpolated_normal);
	record.material = this->m_;
	record.point = ray.at(t);

	return true;
}


bool Triangle::boundingBox(AABB& output) const
{
	auto min_x = std::min(v0_.x(), std::min(v1_.x(), v2_.x()));
	auto min_y = std::min(v0_.y(), std::min(v1_.y(), v2_.y()));
	auto min_z = std::min(v0_.z(), std::min(v1_.z(), v2_.z()));

	auto max_x = std::max(v0_.x(), std::max(v1_.x(), v2_.x()));
	auto max_y = std::max(v0_.y(), std::max(v1_.y(), v2_.y()));
	auto max_z = std::max(v0_.z(), std::max(v1_.z(), v2_.z()));

	// Add padding so the box has nonzero width
	if (min_x == max_x) {
		min_x -= 0.0001;
		max_x += 0.0001;
	}
	else if (min_y == max_y) {
		min_y -= 0.0001;
		max_y += 0.0001;
	}
	else if (min_z == max_z) {
		min_z -= 0.0001;
		max_z += 0.0001;
	}

	output = AABB(Point3(min_x, min_y, min_z), Point3(max_x, max_y, max_z));
	return true;
}

} // namespace rudnick_rt
