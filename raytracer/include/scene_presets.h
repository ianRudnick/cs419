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
#include "bvh_tree.h"
#include "hittable.h"
#include "hittable_list.h"
#include "instance.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "triangle_mesh.h"
#include "utils.h"
#include "vec3.h"


namespace rudnick_rt {

/**
 * Generates a simple scene with spheres, planes, and triangles.
 * @return A HittableList to render.
 */
HittableList basicScene()
{
    HittableList world;

    // Render the ground as a giant rectangle in the xy-plane.
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
 * Generates a scene made up of a bunch of random spheres in a square.
 * Uses a BVH to hold all the spheres.
 * @param size The number of spheres along each side of the square.
 * @return A HittableList containing all the objects in the scene.
 */
HittableList randomSphereScene(int size)
{
    HittableList spheres;
    HittableList world;

    // Render the ground as a giant rectangle in the xy-plane.
    RGBColor ground_color(0.1, 0.1, 0.1);
    auto ground_material = make_shared<BasicLambertian>(ground_color);
    world.add(make_shared<XZRect>(-160, 160, -160, 160, 0, ground_material));

    // Generate the spheres
    for (int x = -size; x < size; x++) {
        for (int z = -size; z < size; z++) {
            // Pick a random location offset for the sphere
            Point3 center(x + 0.9*randomDouble(), 0.2, z + 0.9*randomDouble());

            RGBColor sphere_color = RGBColor::random() * RGBColor::random();
            auto sphere_material = make_shared<BasicLambertian>(sphere_color);
            spheres.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
    }

    // Make all the little spheres into one BVH.
    world.add(make_shared<BVHTree>(spheres));

    return world;
}


/**
 * Generates a scene with the blender monkey.
 */
HittableList monkeyScene()
{
    HittableList world;

    // Render the ground as a giant rectangle in the xy-plane.
    RGBColor ground_color(0.1, 0.1, 0.1);
    auto ground_material = make_shared<BasicLambertian>(ground_color);
    world.add(make_shared<XZRect>(-10, 10, -10, 10, -1, ground_material));

    // Add a monkey
    RGBColor monkey_color(0.2, 0.7, 0.2);
    auto monkey_material = make_shared<BasicLambertian>(monkey_color);
    world.add(make_shared<TriangleMesh>("./data/objects/dragon.obj", monkey_material));

    return world;
}


/**
 * Test scene for materials and instancing
 */
HittableList threeCows()
{
    HittableList world;

    RGBColor green(0.1, 0.1, 0.1);
    auto ground_material = make_shared<BasicMetal>(green, 0.0);
    world.add(make_shared<XZRect>(-10, 10, -10, 10, -0.55, ground_material));

    RGBColor orange(140.0/255.0, 90.0/255.0, 40.0/255.0);
    RGBColor gray(0.6, 0.6, 0.6);
    RGBColor white(1.0, 1.0, 1.0);
    auto m_diffuse = make_shared<BasicLambertian>(orange);
    auto m_metal = make_shared<BasicMetal>(gray, 0.2);
    auto m_glass = make_shared<BasicLambertian>(white);

    auto diffuse_cow =
        make_shared<TriangleMesh>("./data/objects/cow.obj", m_diffuse);
    auto metal_cow =
        make_shared<TriangleMesh>("./data/objects/cow.obj", m_metal);
    auto glass_cow =
        make_shared<TriangleMesh>("./data/objects/cow.obj", m_glass);

    auto translate_metal_cow =
        make_shared<Translate>(metal_cow, Vec3(0.3, 0, 1.5));
    auto translate_glass_cow =
        make_shared<Translate>(glass_cow, Vec3(-0.3, 0, -1.5));

    world.add(diffuse_cow);
    world.add(translate_metal_cow);
    world.add(translate_glass_cow);

    return world;
}


/**
 * Scene using emissive material for area light
 */
HittableList cowApartment()
{
    HittableList world;

    RGBColor floor_color(204.0/255.0, 167.0/255.0, 102.0/255.0);
    auto floor_material = make_shared<BasicLambertian>(floor_color);
    auto floor = make_shared<XZRect>(-5, 5, -5, 5, -0.55, floor_material);
    world.add(floor);

    RGBColor wall_color(0.6, 0.6, 0.6);
    auto wall_material = make_shared<BasicLambertian>(wall_color);
    auto wall1 = make_shared<XYRect>(-5, 5, -0.55, 4, -5, wall_material);
    auto wall2 = make_shared<XYRect>(-5, 5, -0.55, 4, 5, wall_material);
    auto wall3 = make_shared<YZRect>(-0.55, 4, -5, 5, -5, wall_material);
    auto wall4 = make_shared<YZRect>(-0.55, 4, -5, 5, 5, wall_material);
    world.add(wall1);
    world.add(wall2);
    world.add(wall3);
    world.add(wall4);

    RGBColor light_color(1.0, 0.97, 0.85);
    auto light_material = make_shared<RGBColorLight>(light_color);
    auto light1 = make_shared<XYRect>(-4, -2, 0, 3, 4.99, light_material);
    auto light2 = make_shared<XYRect>(-1, 1, 0, 3, 4.99, light_material);
    auto light3 = make_shared<XYRect>(2, 4, 0, 3, 4.99, light_material);
    auto light4 = make_shared<YZRect>(0, 3, -4, -2, 4.99, light_material);
    auto light5 = make_shared<YZRect>(0, 3, -1, 1, 4.99, light_material);
    auto light6 = make_shared<YZRect>(0, 3, 2, 4, 4.99, light_material);
    //world.add(light1);
    world.add(light2);
    world.add(light3);
    //world.add(light4);
    world.add(light5);
    world.add(light6);

    RGBColor ceil_color(1.0, 1.0, 1.0);
    auto ceil_material = make_shared<BasicLambertian>(ceil_color);
    auto ceil = make_shared<XZRect>(-5, 5, -5, 5, 4, ceil_material);
    world.add(ceil);

    RGBColor cow_color(140.0/255.0, 90.0/255.0, 40.0/255.0);
    auto cow_material = make_shared<BasicLambertian>(cow_color);
    auto cow = make_shared<TriangleMesh>("./data/objects/cow.obj", cow_material);
    world.add(cow);

    // throw in some spheres to make it more interesting
    HittableList spheres;

    auto size = 4;
    auto y_coord = -0.55 + 0.2;

    // Generate the spheres
    for (int x = -size; x < size; x++) {
        for (int z = -size; z < size; z++) {
            // Pick a random material and location for the sphere
            auto choose_material = randomDouble();
            Point3 center(x + 0.9*randomDouble(), y_coord, z + 0.9*randomDouble());

            // Leave some space in the center to draw the cow
            if ((center + Point3(0, y_coord, 0)).length() > 1.0) {
                shared_ptr<Material> sphere_material;

                if (choose_material < 0.6) {
                    // Make a Lambertian sphere
                    auto albedo = RGBColor::random() * RGBColor::random();
                    sphere_material = make_shared<BasicLambertian>(albedo);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else if (choose_material < 0.8) {
                    // Make a metal sphere
                    auto albedo = RGBColor::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<BasicMetal>(albedo, fuzz);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else if (choose_material < 0.9) {
                    // Make a solid glass sphere
                    auto albedo = RGBColor::random(0.9, 1);
                    sphere_material = make_shared<BasicDielectric>(albedo,1.5);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else {
                    // Make a hollow glass sphere
                    RGBColor albedo(1.0, 1.0, 1.0);
                    sphere_material = make_shared<BasicDielectric>(albedo,1.5);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                    spheres.add(
                        make_shared<Sphere>(center,-0.18,sphere_material)
                    );
                }
            }
        }
    }

    // Make all the little spheres into one BVH.
    world.add(make_shared<BVHTree>(spheres));

    return world;
}


/**
 * Simpler scene to show just the area light
 */
HittableList areaLight()
{
    HittableList world;

    RGBColor floor_color(0.4, 0.4, 0.4);
    auto floor_material = make_shared<BasicLambertian>(floor_color);
    auto floor = make_shared<XZRect>(-10, 10, -10, 10, -0.55, floor_material);
    world.add(floor);

    RGBColor light_color(1.0, 0.97, 0.85);
    light_color *= 4.0;
    auto light_material = make_shared<RGBColorLight>(light_color);
    auto light1 = make_shared<XYRect>(-4, -2, 0, 3, 4.99, light_material);
    auto light2 = make_shared<XYRect>(-1, 1, 0, 3, 4.99, light_material);
    auto light3 = make_shared<XYRect>(2, 4, 0, 3, 4.99, light_material);
    auto light4 = make_shared<YZRect>(0, 3, -4, -2, 4.99, light_material);
    auto light5 = make_shared<YZRect>(0, 3, -1, 1, 4.99, light_material);
    auto light6 = make_shared<YZRect>(0, 3, 2, 4, 4.99, light_material);
    //world.add(light1);
    //world.add(light2);
    //world.add(light3);
    //world.add(light4);
    world.add(light5);
    //world.add(light6);

    // add a backing ot the light so it doesn't illuminate things behind it
    auto back_of_light = make_shared<YZRect>(0, 3, -1, 1, 5, floor_material);
    world.add(back_of_light);

    RGBColor cow_color(140.0/255.0, 90.0/255.0, 40.0/255.0);
    auto cow_material = make_shared<BasicLambertian>(cow_color);
    auto cow = make_shared<TriangleMesh>("./data/objects/cow.obj", cow_material);
    world.add(cow);

    // throw in some spheres to make it more interesting
    HittableList spheres;

    auto size = 4;
    auto y_coord = -0.55 + 0.2;

    // Generate the spheres
    for (int x = -size; x < size; x++) {
        for (int z = -size; z < size; z++) {
            // Pick a random material and location for the sphere
            auto choose_material = randomDouble();
            Point3 center(x + 0.9*randomDouble(), y_coord, z + 0.9*randomDouble());

            // Leave some space in the center to draw the cow
            if ((center + Point3(0, y_coord, 0)).length() > 1.0) {
                shared_ptr<Material> sphere_material;

                if (choose_material < 0.6) {
                    // Make a Lambertian sphere
                    auto albedo = RGBColor::random() * RGBColor::random();
                    sphere_material = make_shared<BasicLambertian>(albedo);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else if (choose_material < 0.8) {
                    // Make a metal sphere
                    auto albedo = RGBColor::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<BasicMetal>(albedo, fuzz);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else if (choose_material < 0.9) {
                    // Make a solid glass sphere
                    auto albedo = RGBColor::random(0.9, 1);
                    sphere_material = make_shared<BasicDielectric>(albedo,1.5);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                }
                else {
                    // Make a hollow glass sphere
                    RGBColor albedo(1.0, 1.0, 1.0);
                    sphere_material = make_shared<BasicDielectric>(albedo,1.5);
                    spheres.add(make_shared<Sphere>(center,0.2,sphere_material));
                    spheres.add(
                        make_shared<Sphere>(center,-0.18,sphere_material)
                    );
                }
            }
        }
    }

    // Make all the little spheres into one BVH.
    world.add(make_shared<BVHTree>(spheres));

    return world;
}

} // namespace rudnick_rt

#endif
