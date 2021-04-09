/**
 * @file bvh_tree.cpp
 * @author Ian Rudnick
 * Implementation of the Bounding Volume Hierarchy tree.
 */
#include "bvh_tree.h"

#include <algorithm>
#include <iostream>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "utils.h"


namespace rudnick_rt {
//-----------------------------------------------------------------------------
// BVHTree

BVHTree::BVHTree() {}

BVHTree::BVHTree(const HittableList& list) {
    root_ = std::make_shared<BVHNode>(list.objects_, 0, list.objects_.size());
}

bool BVHTree::hit(const Ray& ray, double tmin, double tmax,
                  hit_record& record) const {
    return root_->hit(ray, tmin, tmax, record);
}

bool BVHTree::boundingBox(AABB& box) const {
    return root_->boundingBox(box);
}

//-----------------------------------------------------------------------------
// BVHNode

BVHTree::BVHNode::BVHNode(const std::vector<shared_ptr<Hittable>> & objects,
                          size_t start, size_t end) {
    // Create a modifiable local copy of the objects in the scene.
    auto objects_copy = objects;

    // Fill out this node's subtrees.
    // If there's only one object, put it in both subtrees.
    size_t num_objects = end - start;
    if (num_objects == 1) {
        left_ = objects_copy[start];
        right_ = objects_copy[start];
    }
    // If there's two objects, put one in each subtree.
    else if (num_objects == 2) {
        left_ = objects_copy[start];
        right_ = objects_copy[start + 1];
    }
    // If there's more than two, recursively build the rest of the tree.
    else {
        // Find the bounds of the objects' centroids.
        AABB centroid_bounds;
        AABB node_box;
        for (size_t i = start; i < end; ++i) {
            objects[i].get()->boundingBox(node_box);
            auto node_centroid = node_box.centroid();
            AABB node_centroid_box(node_centroid, node_centroid);
            centroid_bounds = AABB::surroundingBox(centroid_bounds, node_centroid_box);
        }

        // Choose the axis where the centroids have maximum extent.
        auto extent = centroid_bounds.max() - centroid_bounds.min();
        int axis;
        if (extent.x() > extent.y() && extent.x() > extent.z())
            axis = 0;
        else if (extent.y() > extent.z())
            axis = 1;
        else
            axis = 2;

        // Comparator for the std::partition
        auto comparator = 
            [axis, centroid_bounds](const std::shared_ptr<Hittable>& object) -> bool {
                AABB object_bounds;
                object.get()->boundingBox(object_bounds);

                // Return true if the object's centroid is below the midpoint.
                return object_bounds.centroid()[axis] < centroid_bounds.centroid()[axis];
            };
        
        // Partition the objects based on the comparator.
        auto partition_iter = std::partition(objects_copy.begin()+start, 
                                             objects_copy.begin()+end,
                                             comparator);

        size_t partition_point = partition_iter - objects_copy.begin();
        
        if (partition_point == start || partition_point == end) {
            partition_point = start + (end - start) / 2;
        }
        //std::cout << start << " " << partition_point << " " << end << "\n";

        left_ = make_shared<BVHNode>(objects_copy, start, partition_point);
        right_ = make_shared<BVHNode>(objects_copy, partition_point, end);
    }

    AABB left_box, right_box;
    if (!left_->boundingBox(left_box) || !right_->boundingBox(right_box)) {
        std::cerr << "No bounding box in BVHNode constructor." << std::endl;
    }

    // Set the box at this node to surround the boxes of each subtree.
    this->box_ = AABB::surroundingBox(left_box, right_box);
}


bool BVHTree::BVHNode::hit(
    const Ray & ray, double tmin, double tmax, hit_record & record) const
{
    // If the ray doesn't hit this box, it doesn't hit the boxes inside either
    if (!box_.hit(ray, tmin, tmax)) {
        return false;
    }

    // If the ray does hit this box, check if it hits any boxes inside this one
    bool hit_left = left_->hit(ray, tmin, tmax, record);
    bool hit_right = right_->hit(ray, tmin, hit_left?record.t:tmax, record);
    
    return hit_left || hit_right;
}

bool BVHTree::BVHNode::boundingBox(AABB & box) const {
    box = box_;
    return true;
}
 
} // namespace rudnick_rt

