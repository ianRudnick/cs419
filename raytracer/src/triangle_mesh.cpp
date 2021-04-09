/**
 * @file triangle_mesh.cpp
 * @author Ian Rudnick
 * @brief my janky class woo
 */
#include "triangle_mesh.h"

#include <memory>
#include <string>
#include <vector>

#include "hittable_list.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


namespace rudnick_rt {

TriangleMesh::TriangleMesh(const std::string& filename,
						   std::shared_ptr<Material> mat)
{
	// load mesh into vector of vertices
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool load_successful =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), 0);

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (load_successful) {
		size_t num_vertices = attrib.vertices.size() / 3;

		// Create a vector of the vertex positions.
		std::vector<Point3> positions;
		for (size_t i = 0; i < num_vertices; ++i) {
			positions.push_back(Point3(attrib.vertices[i*3+0],
									   attrib.vertices[i*3+1],
									   attrib.vertices[i*3+2]));
		}

		// Create a vector for the vertex indices
		std::vector<int> indices;
		// For every shape, for every face, push the vertex indices to the buffer
		for (size_t s = 0; s < shapes.size(); s++) {
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

				for (size_t v = 0; v < 3; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[3 * f + v];
					indices.push_back(idx.vertex_index);
				}
			}
		}
		size_t num_triangles = indices.size() / 3;

		// Create a vector for the vertex normals.
		std::vector<Vec3> normals;
		normals.resize(num_vertices);

		// Loop over each face.
		for (size_t i = 0; i < num_triangles; ++i) {
			// Compute the normal as the cross product of the three vertices.
			auto e1 = positions[indices[i*3+1]] - positions[indices[i*3+0]];
			auto e2 = positions[indices[i*3+2]] - positions[indices[i*3+0]];
			Vec3 e1_cross_e2 = Vec3::cross(e1, e2);
			Vec3 face_normal = Vec3::normalize(e1_cross_e2);

			// Scale the normal by the triangle's surface area.
			face_normal *= e1_cross_e2.length()/2;

			// Add the face normal to the normals of the three vertices.
			normals[indices[i*3+0]] += face_normal;
			normals[indices[i*3+1]] += face_normal;
			normals[indices[i*3+2]] += face_normal;
		}

		// Loop over each vertex normal and normalize them.
		for (size_t i = 0; i < num_vertices; ++i) {
			normals[i] = Vec3::normalize(normals[i]);
		}

		// Create the triangle primitives and add them to the HittableList.
		HittableList triangles;
		for (size_t i = 0; i < num_triangles; ++i) {
			int idx0 = indices[i*3+0];
			int idx1 = indices[i*3+1];
			int idx2 = indices[i*3+2];
			triangles.add(std::make_shared<Triangle>(positions[idx0],
													 positions[idx1],
													 positions[idx2],
													 normals[idx0],
													 normals[idx1],
													 normals[idx2],
													 mat));
		}

		this->mesh_ = std::make_shared<BVHTree>(triangles);
		
	}
	else {
		std::cerr << "WARNING: Could not load OBJ file " << filename << std::endl;
	}
}


bool TriangleMesh::hit(
	const Ray & ray, double tmin, double tmax, hit_record & record) const
{
	return mesh_->hit(ray, tmin, tmax, record);
}


bool TriangleMesh::boundingBox(AABB& output) const
{
	return mesh_->boundingBox(output);
}

}
