/**
 * @file bvh_tree.cpp
 * @author Ian Rudnick
 * Implementation of the Bounding Volume Hierarchy tree.
 */

#include "bvh_tree.h"

#include <algorithm>
#include <iostream>

namespace rudnick_rt {
//-----------------------------------------------------------------------------
// BVHTree

BVHTree::BVHTree() {}

BVHTree::BVHTree(const HittableList & list) {
    root_ = make_shared<BVHNode>(list.objects_, 0, list.objects_.size());
}

bool BVHTree::hit(const Ray & ray, double tmin, double tmax,
                  hit_record & record) const {
    return root_->hit(ray, tmin, tmax, record);
}

bool BVHTree::boundingBox(AABB & box) const {
    return root_->boundingBox(box);
}

//-----------------------------------------------------------------------------
// BVHNode

BVHTree::BVHNode::BVHNode(const std::vector<shared_ptr<Hittable>> & objects,
                          size_t start, size_t end) {
    // Make a modifiable local copy of the objects in the scene.
    auto _objects = objects;

    // Choose a random axis.
    // Get the address of the function to compare that axis.
    int axis = randomInt(0, 2);
    auto comparator = (axis == 0) ? BVHTree::BVHNode::compareBoxX
                    : (axis == 1) ? BVHTree::BVHNode::compareBoxY
                    :               BVHTree::BVHNode::compareBoxZ;

    // If there's only one object, put it in both subtrees.
    size_t num_objects = end - start;
    if (num_objects == 1) {
        left_ = _objects[start];
        right_ = _objects[start];
    }
    // If there's two objects, compare and put one in each subtree.
    else if (num_objects == 2) {
        if (comparator(_objects[start], _objects[start+1])) {
            left_ = _objects[start + 1];
            right_ = _objects[start];
        }
        else {
            left_ = _objects[start];
            right_ = _objects[start + 1];
        }
    }
    // If there's more than two, recursively build the rest of the tree.
    else {
        std::sort(_objects.begin()+start, _objects.begin()+end, comparator);
        auto middle = start + num_objects/2;
        left_ = make_shared<BVHNode>(_objects, start, middle);
        right_ = make_shared<BVHNode>(_objects, middle, end);
    }

    AABB left_box, right_box;
    if (!left_->boundingBox(left_box) || !right_->boundingBox(right_box)) {
        std::cerr << "No bounding box in BVHNode constructor." << std::endl;
    }

    // Set the box at this node to surround the boxes of each subtree.
    box_ = AABB::surroundingBox(left_box, right_box);
}

bool BVHTree::BVHNode::hit(const Ray & ray, double tmin, double tmax,
                           hit_record & record) const {

    // If the ray doesn't hit this box, it doesn't hit the boxes inside either
    if (!box_.hit(ray, tmin, tmax)) {
        return false;
    }

    // If the ray does hit this box, check if it hits any boxes inside this one
    bool hit_left = left_->hit(ray, tmin, tmax, record);
    if (!hit_left) {
        bool hit_right = right_->hit(ray, tmin, hit_left?record.t:tmax, record);
        return hit_right;
    }
    return hit_left;
}

bool BVHTree::BVHNode::boundingBox(AABB & box) const {
    box = box_;
    return true;
}

bool BVHTree::BVHNode::compareBox(const shared_ptr<Hittable> a,
                                  const shared_ptr<Hittable> b,
                                  int axis) {
    AABB box_a;
    AABB box_b;
    if (!a->boundingBox(box_a) || !b->boundingBox(box_b)) {
        std::cerr << "No bounding box in BVHNode constructor." << std::endl;
    }
    return box_a.min()[axis] < box_b.min()[axis];
}
 
} // namespace rudnick_rt

