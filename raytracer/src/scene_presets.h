/**
 * @file scene_presets.h
 * @author Ian Rudnick
 * Functions to build preset scenes for the raytracer.
 * 
 * For CS 419 at the University of Illinois at Urbana-Champaign.
 */
#ifndef RUDNICKRT_SCENE_PRESETS_H
#define RUDNICKRT_SCENE_PRESETS_H

#include <iostream>
#include <cmath>

#include "aa_rectangle.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "utils.h"
#include "vec3.h"


namespace rudnick_rt {

/**
 * Generates a simple scene with spheres, planes, and triangles.
 * @return A HittableList to render.
 */
HittableList basicScene() {
    HittableList world;

    // Render the ground as a giant sphere
    auto ground_material = 
        make_shared<BasicLambertian>(RGBColor(0.7, 1.0, 0.5));
    world.add(make_shared<XZRect>(-1000, 1000, -1000, 1000, 0, ground_material));

    // Jumbo lambertian sphere
    auto blue = make_shared<BasicLambertian>(RGBColor(0.1, 0.2, 0.4));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, blue));
    // Jumbo metal sphere
    auto purple = make_shared<BasicLambertian>(RGBColor(0.7, 0.1, 0.6));
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, purple));

    // set up some triangle vertices
    Point3 vertex1(1, 0, 1);
    Point3 vertex2(-1, 0, 1);
    Point3 vertex3(0, 0, -1);
    Point3 top(0, 3, 0);

    // Tetrahedron
    auto orange = make_shared<BasicLambertian>(RGBColor(1.0, 0.8, 0.2));
    world.add(make_shared<Triangle>(vertex1, vertex2, top, orange));
    world.add(make_shared<Triangle>(vertex2, vertex3, top, orange));
    world.add(make_shared<Triangle>(vertex3, vertex1, top, orange));

    return world;
}

/**
 * Scene with more interesting tetrahedrons.
 * @return A HittableList to render.
 */
HittableList tetraScene() {
    HittableList world;

    // Render the ground as a giant sphere
    auto ground_material = 
        make_shared<BasicLambertian>(RGBColor(0.7, 1.0, 0.5));
    world.add(make_shared<XZRect>(-1000, 1000, -1000, 1000, 0, ground_material));

    // Jumbo lambertian sphere
    auto blue = make_shared<BasicLambertian>(RGBColor(0.1, 0.2, 0.4));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, blue));
    // Jumbo metal sphere
    auto purple = make_shared<BasicLambertian>(RGBColor(0.7, 0.2, 0.9));
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, purple));

    // set up some triangle vertices
    Point3 t1v1(1, 0, 1);
    Point3 t1v2(-1, 0, 1);
    Point3 t1v3(0, 0, -1);
    Point3 t1v4(0, 3, 0);

    Point3 t2v1(3, 5, 2);
    Point3 t2v2(-2.7, 5, 1.8);
    Point3 t2v3(-1.3, 5, -2.4);
    Point3 t2v4(0, 2.5, 0);

    // Tetrahedrons
    auto white = make_shared<BasicLambertian>(RGBColor(0.98, 0.97, 0.99));
    world.add(make_shared<Triangle>(t1v1, t1v2, t1v4, white));
    world.add(make_shared<Triangle>(t1v2, t1v3, t1v4, white));
    world.add(make_shared<Triangle>(t1v3, t1v1, t1v4, white));

    world.add(make_shared<Triangle>(t2v1, t2v2, t2v4, white));
    world.add(make_shared<Triangle>(t2v2, t2v3, t2v4, white));
    world.add(make_shared<Triangle>(t2v3, t2v1, t2v4, white));

    return world;
}

} // namespace rudnick_rt

#endif
