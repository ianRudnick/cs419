/**
 * @file triangle_mesh.h
 * @author Ian Rudnick
 * @brief Class representing a triangulated mesh. Can be loaded in from an OBJ
 * file. Derived from the Hittable class.
 */
#ifndef RUDNICKRT_TRIANGLE_MESH_H
#define RUDNICKRT_TRIANGLE_MESH_H

#include <memory>
#include <string>

#include "aabb.h"
#include "bvh_tree.h"
#include "hittable.h"
#include "hittable_list.h"
#include "triangle.h"

namespace rudnick_rt {

class TriangleMesh: public Hittable {
public:
	/**
	 * Default constructor.
	 * Does nothing. Don't use this.
	 */
	TriangleMesh() {}

	/**
	 * Constructs a TriangleMesh from an OBJ file.
	 * @param filename Name of the .obj file containing the mesh.
	 */
	TriangleMesh(const std::string& filename, std::shared_ptr<Material> mat);

	virtual bool hit(const Ray & ray, double tmin, double tmax, 
					 hit_record & record) const override;

	virtual bool boundingBox(AABB& output) const override;

private:
	std::shared_ptr<BVHTree> mesh_;
};

}

#endif
