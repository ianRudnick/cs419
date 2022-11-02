/**
 * @file bvh_tree.h
 * @author Ian Rudnick
 * Bounding Volume Hierarchy tree class for use in raytracing calculations.
 * Uses a class for the tree, and a nested class for the nodes.
 * 
 * Based on Peter Shirley's Implementation in Ray Tracing: The Next Week,
 * and Physically Based Rendering.
 */
#ifndef RUDNICKRT_BVH_TREE_H
#define RUDNICKRT_BVH_TREE_H

#include <vector>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "utils.h"


namespace rudnick_rt {

class BVHTree : public Hittable {
//-----------------------------------------------------------------------------
// BVH node class
public:
    class BVHNode : public Hittable {
    public:
        /**
         * Constructs a default BVH Node.
         */
        BVHNode() {}

        /**
         * Constructs a BVH Node and fills out its children from a vector of
         * Hittables.
         * @param objects The vector of objects to determine the BVH.
         * @param start
         * @param end
         */
        BVHNode(const std::vector<shared_ptr<Hittable>> & objects,
                size_t start, size_t end);

        /**
         * Determines whether a given ray hits the box at this node.
         * @param ray The ray to check for a hit.
         * @param tmin The minimum distance along the ray to detect a hit.
         * @param tmax The maximum distance along the ray to detect a hit.
         * @param record A hit_record to store the details of what was hit.
         * @return True if the ray hits the box.
         */
        virtual bool hit(const Ray & ray, double tmin, double tmax,
                         hit_record & record) const override;

        /**
         * Gets the bounding box of a BVH Node.
         * @param box Output to store the bounding box.
         * @return True if successful.
         */
        virtual bool boundingBox(AABB& box) const override;
        
    private:
        shared_ptr<Hittable> left_;
        shared_ptr<Hittable> right_;
        AABB box_;
    };

//-----------------------------------------------------------------------------
// BVH Tree Methods

public:
    /**
     * Constructs an empty BVH Tree.
     */
    BVHTree();

    /**
     * Constructs a BVH Tree from a Hittable List.
     * @param list The Hittable List to determine the BVH.
     */
    BVHTree(const HittableList & list);

    /**
     * Determines whether a given ray hits any objects in the tree.
     * @param ray The ray to check for a hit.
     * @param tmin The minimum distance along the ray to detect a hit.
     * @param tmax The maximum distance along the ray to detect a hit.
     * @param record A hit_record to store the details of what was hit.
     * @return True if the ray hits anything.
     */
    virtual bool hit(const Ray & ray, double tmin, double tmax, 
                     hit_record & record) const override;

    /**
     * Gets the bounding box of a BVH Tree.
     * @param box Output to store the bounding box.
     * @return True if successful.
     */
    virtual bool boundingBox(AABB & box) const override;

private:
    shared_ptr<BVHNode> root_;

};
   
} // namespace rudnick_rt

#endif
