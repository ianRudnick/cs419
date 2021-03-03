/**
 * @file camera.cpp
 * @author Ian Rudnick
 * Implementation of the virtual camera class for the raytracer.
 */

#include "camera.h"

#include <iostream>
#include "rrt_enum.h"


namespace rudnick_rt {

Camera::Camera() {
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin_ = Point3(0, 0, 0);
    horizontal_ = Vec3(viewport_width, 0.0, 0.0);
    vertical_ = Vec3(0.0, viewport_height, 0.0);
    lower_left_corner_ = origin_ - horizontal_/2.0 - vertical_/2.0
                       - Vec3(0, 0, focal_length);
}

Camera::Camera(Point3 viewpt, Point3 lookat, Vec3 up, double fov,
               double aspect_ratio, double aperture, double focus_dist) {

    auto fov_rad = degToRad(fov);
    auto h = tan(fov_rad / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;
    
    // Calculate axes for the camera in view coordinates
    w_ = Vec3::normalize(viewpt - lookat);
    u_ = Vec3::normalize(Vec3::cross(up, w_));
    v_ = Vec3::cross(w_, u_);

    origin_ = viewpt;
    horizontal_ = focus_dist * viewport_width * u_;
    vertical_ = focus_dist * viewport_height * v_;
    lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - focus_dist*w_;
    lens_radius_ = aperture / 2;
}

Ray Camera::getRay(double s, double t, RRTenum projection) const {
    Vec3 ray_origin;
    Vec3 ray_direction;

    // Calculate the position of the screen pixel, in view space.
    Point3 screen_pixel = lower_left_corner_ + s*horizontal_ + t*vertical_;
    
    if (projection == RRTenum::PERSPECTIVE) {
        ray_origin = origin_;
        ray_direction = screen_pixel - origin_;
    }
    else if (projection == RRTenum::ORTHOGRAPHIC) {
        ray_origin = screen_pixel;
        ray_direction = -w_;
    }
    return Ray(ray_origin, ray_direction);
}
    
} // namespace rudnick_rt
