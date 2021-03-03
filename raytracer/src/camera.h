/**
 * @file camera.h
 * @author Ian Rudnick
 * Camera class to manage the virtual camera and related tasks.
 */

#ifndef RUDNICKRT_CAMERA_H
#define RUDNICKRT_CAMERA_H

#include "ray.h"
#include "rrt_enum.h"
#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {

class Camera {
public:
    /**
     * Constructs a new camera object.
     * Initializes the camera with a default view.
     */
    Camera();

    /**
     * Constructs a new camera object with the given view parameters.
     * @param viewpt The position of the camera in world coordinates.
     * @param lookat The point to aim the camera at.
     * @param up Vector indicating which direction is up.
     * @param fov The vertical field of view, in degrees.
     * @param aspect_ratio The aspect ratio of the screen.
     */
    Camera(Point3 viewpt, Point3 lookat, Vec3 up, double fov,
           double aspect_ratio, double aperture, double focus_dist);

    /**
     * Gets a viewing ray.
     * @param s The horizontal coordinate to point to.
     * @param t The vertical coordinate to point to.
     * @param projection The projection mode to use. Either PERSPECTIVE
     *                   or ORTHOGRAPHIC.
     * @return A ray from the origin to the given screen coordinates.
     */
    Ray getRay(double s, double t, RRTenum projection) const;

private:
    Point3 origin_;
    Point3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 u_, v_, w_;
    double lens_radius_;

}; // class Camera
    
} // namespace rudnick_rt


#endif