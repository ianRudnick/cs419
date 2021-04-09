#include "instance.h"

#include <cmath>

#include "ray.h"
#include "utils.h"

namespace rudnick_rt {

//-----------------------------------------------------------------------------
// Translate
bool Translate::hit(
    const Ray & ray, double tmin, double tmax, hit_record & record
) const {
    Ray moved(ray.origin() - displacement_, ray.direction());
    if (!ptr_->hit(moved, tmin, tmax, record))
        return false;
    
    record.point += displacement_;
    record.setNormalDirection(moved, record.normal);
    return true;
}


bool Translate::boundingBox(AABB & box) const {
    if (!ptr_->boundingBox(box))
        return false;

    box = AABB(box.min() + displacement_, box.max() + displacement_);
    return true;
}


//-----------------------------------------------------------------------------
// Rotate Y
RotateY::RotateY(shared_ptr<Hittable> p, double angle) : ptr_(p) {
    auto radians = degToRad(angle);
    sin_ = std::sin(radians);
    cos_ = std::cos(radians);
    has_box_ = ptr_->boundingBox(box_);

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                auto x = i*box_.max().x() + (1-i)*box_.min().x();
                auto y = j*box_.max().y() + (1-j)*box_.min().y();
                auto z = k*box_.max().z() + (1-k)*box_.min().z();

                auto newx = cos_ * x + sin_ * z;
                auto newz = -sin_ * x + cos_ * z;

                // Figure out the bounding box
                Vec3 test(newx, y, newz);
                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], test[c]);
                    max[c] = fmax(max[c], test[c]);
                }
            }
        }
    }

    box_ = AABB(min, max);
}


bool RotateY::hit(
    const Ray & ray, double tmin, double tmax, hit_record & record
) const {
    auto origin = ray.origin();
    auto direction = ray.direction();
    origin[0] = cos_*ray.origin()[0] - sin_*ray.origin()[2];
    origin[2] = sin_*ray.origin()[0] + cos_*ray.origin()[2];
    direction[0] = cos_*ray.direction()[0] - sin_*ray.direction()[2];
    direction[2] = sin_*ray.direction()[0] + cos_*ray.direction()[2];

    Ray rotated(origin, direction);
    if (!ptr_->hit(rotated, tmin, tmax, record))
        return false;

    auto point = record.point;
    auto normal = record.normal;
    point[0] = cos_*record.point[0] + sin_*record.point[2];
    point[2] = -sin_*record.point[0] + cos_*record.point[2];
    normal[0] = cos_*record.normal[0] + sin_*record.normal[2];
    normal[2] = -sin_*record.normal[0] + cos_*record.normal[2];

    record.point = point;
    record.setNormalDirection(rotated, normal);
    return true;
}


bool RotateY::boundingBox(AABB & box) const {
    box = box_;
    return has_box_;
}


//-----------------------------------------------------------------------------
// Recolor
bool Recolor::hit(
    const Ray& ray, double tmin, double tmax, hit_record& record
) const {
    bool hit_result = ptr_->hit(ray, tmin, tmax, record);
    // Overwrite the hit material with the recolor's material
    record.material = this->mat_;
    return hit_result;
}


bool Recolor::boundingBox(AABB& box) const {
    return ptr_->boundingBox(box);
}
    
} // namespace rudnick_rt
