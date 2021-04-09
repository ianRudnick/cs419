/**
 * @file renderer.cpp
 * @author Ian Rudnick
 * Main rendering engine for the raytracer.
 * Based on Peter Shirley's Implementation in Ray Tracing In One Weekend.
 * 
 * For CS 419 at the University of Illinois at Urbana-Champaign.
 */
#include <iostream>
#include <cmath>
#include <string>

#include "png.h"
#include "rgba_pixel.h"

#include "aa_rectangle.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "rrt_enum.h"
#include "scene_presets.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

using namespace rudnick_rt;


namespace rudnick_rt {

typedef struct {
    double x, y;
} Point2;

/**
 * Non-recursive helper function to trace a ray and determine what color it
 * hits.
 * @param ray The ray to trace.
 * @param background The background color.
 * @param world The scene to render and check for ray intersections.
 * @return The RGBColor to use to color the pixel.
 */
RGBColor traceRayPhong(const Ray & ray, const RGBColor & background,
                  const Hittable & world) {
    // Put a point light source at (8, 8, 8)
    Point3 light_source(7, 10, 4);
    // Record what the ray hits
    hit_record record;
    
    // If the ray hit something, do basic diffuse lighting
    if (world.hit(ray, 0.001, infinity, record)) {
        // Get the color of the object we hit
        RGBColor object_color;
        Ray scattered;
        record.material->scatter(ray, record, object_color, scattered);

        // Ambient weighting is constant.
        auto ambient_weight = 0.1;
        
        // Compute the weighting for diffuse light component.
        Vec3 L = light_source - record.point;
        auto light_angle = Vec3::dot(Vec3::normalize(L),
                                     Vec3::normalize(record.normal));
        auto diffuse_weight = std::max(light_angle, 0.0);

        // Compute the weighting for specular light component.
        Vec3 R = Vec3::reflect(L, record.normal);
        auto reflect_angle = Vec3::dot(Vec3::normalize(R),
                                       Vec3::normalize(ray.direction()));
        auto a = 3.0;
        auto specular_weight = std::max(pow(reflect_angle, a), 0.0);
        
        // // If there's something blocking the light, make a hard shadow
        // Ray shadow_ray = Ray(record.point, L);
        // auto distance_to_light = L.length();
        // if (world.hit(shadow_ray, 0.001, distance_to_light, record)) {
        //     diffuse_weight *= 0.3;
        // }
        
        return (object_color * ambient_weight)
            + (object_color * diffuse_weight)
            + (object_color * specular_weight);
    }

    // If the ray did not hit anything, color the sky a gradient
    Vec3 direction_normal = Vec3::normalize(ray.direction());
    auto t = 0.5 * (direction_normal.y() + 1.0);
    return (1.0-t) * RGBColor(1.0, 0.9, 0.8) + t * RGBColor(0.4, 0.6, 1.0);
}


/**
 * Recursive helper function to trace a ray and determine what color it hits.
 * Sets the background to a sky-blue gradient.
 * Implements basic antialiasing.
 * @param ray The ray to trace.
 * @param world The scene to render and check for ray intersections.
 * @param depth The max recursion depth/number of ray bounces.
 * @return The RGBColor to use to color the pixel.
 */
RGBColor traceRayRecursive(const Ray & ray, const Hittable & world, int depth) {
    // Record what the ray hits
    hit_record record;

    // Limit the max recursion depth
    if (depth <= 0) {
        return RGBColor(0, 0, 0);
    }

    // If the ray hits anything, shoot a new ray in a random direction, and
    // color based on the new ray
    if (world.hit(ray, 0.001, infinity, record)) {
        Ray scattered;
        RGBColor attenuation;
        if (record.material->scatter(ray, record, attenuation, scattered)) {
            return attenuation * traceRayRecursive(scattered, world, depth - 1);
        }
        return RGBColor(0, 0, 0);
    }

    // If the ray did not hit anything, color the sky a gradient
    Vec3 direction_normal = Vec3::normalize(ray.direction());
    auto t = 0.5 * (direction_normal.y() + 1.0);
    return (1.0-t) * RGBColor(0.8, 0.9, 1.0) + t * RGBColor(0.4, 0.6, 1.0);
}


/**
 * Recursive helper function to trace a ray and determine what color it hits.
 * Implements basic antialiasing.
 * @param ray The ray to trace.
 * @param background The background color.
 * @param world The scene to render and check for ray intersections.
 * @param depth The max recursion depth/number of ray bounces.
 * @return The RGBColor to use to color the pixel.
 */
RGBColor traceRayRecursive(const Ray & ray, const RGBColor & background,
                           const Hittable & world, int depth) {
    hit_record record;

    // Limit the maximum number of bounces
    if (depth <= 0) 
        return RGBColor(0, 0, 0);

    // If the ray doesn't hit anything, color it with the background color
    if (!world.hit(ray, 0.001, infinity, record)) 
        return background;

    Ray scattered;
    RGBColor attenuation;
    RGBColor emitted = record.material->emitted(0, 0, record.point);

    if (!record.material->scatter(ray, record, attenuation, scattered))
        return emitted;

    return emitted +
        attenuation * traceRayRecursive(scattered, background, world, depth-1);
}


/**
 * Creates an array of sample points for multi-jittered sampling.
 * @param p array of point2s; must have cols*rows cells
 * @param cols number of columns
 * @param rows number of rows
 */
void multiJitter(Point2 p[], int cols, int rows) {
    auto subcell_width = 1.0 / (cols * rows);

    // Initialize the array to the canonical multi-jittered pattern.
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            
            p[i*rows + j].x = i*rows*subcell_width + j*subcell_width
                + randomDouble(0, subcell_width);
            p[i*rows + j].y = j*cols*subcell_width + i*subcell_width
                + randomDouble(0, subcell_width);
        }
    }

    // Shuffle x-coordinates within each column of cells.
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            
            auto k = randomInt(j, rows-1);
            auto t = p[i*rows + j].x;
            p[i*rows + j].x = p[i*rows + k].x;
            p[i*rows + k].x = t;
        }
    }

    // Shuffle y-coordinates within each row of cells.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            
            auto k = randomInt(j, cols-1);
            auto t = p[j*rows + i].x;
            p[j*rows + i].y = p[k*rows + i].y;
            p[k*rows + i].y = t;
        }
    }
}

} // namespace rudnick_rt


//-----------------------------------------------------------------------------
/**
 * The main rendering program.
 */
int main() {
    // Request a name for the output file
    std::string render_name;
    std::cout << "Enter a name for the new image: ";
    std::cin >> render_name;
    std::cout << "Rendering " << render_name << ".png" << std::endl;

    // Set up image
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 1280;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 400;  // MUST BE A SQUARE NUMBER
    const int max_depth = 100;
    PNG *render = new PNG(image_width, image_height);
    RGBColor background(0.2, 0.8, 1.0);

    // Set up world
    // 1000 spheres = 32, 10,000 = 50, 100,000 = 159
    HittableList world = areaLight();

    // Set up camera
    //Point3 camera_pos = Point3(-8, 15, -5);
    Point3 camera_pos = Point3(-8, 8, -8);
    Point3 lookat = Point3(0.5, 0.5, 0);
    Vec3 up(0, 1, 0);
    auto fov = 20.0;
    auto focal_distance = 10.0;
    auto aperture = 0.1;
    RRTenum projection = RRTenum::PERSPECTIVE;
    
    Camera cam(camera_pos, lookat, up, fov, aspect_ratio, aperture, focal_distance);

    // Set up a multi-jitter sample pattern
    auto sample_pattern_rows = std::sqrt(samples_per_pixel);
    Point2 sample_pattern[samples_per_pixel];
    multiJitter(sample_pattern, sample_pattern_rows, sample_pattern_rows);

    // Render the image!
    // Loop over each row of pixels (scanline)
    for (int y = 0; y < image_height; ++y) {
        // Show the progress on the console
        std::cout << "\rScanlines remaining: " << image_height - y << ' ' << std::flush;
        
        // For each pixel in the row
        for (int x = 0; x < image_width; ++x) {
    
            // Get the pixel color, with antialiasing
            RGBColor pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (x + sample_pattern[s].x) / (image_width - 1);
                auto v = (y + sample_pattern[s].y) / (image_height - 1);
                Ray ray = cam.getRay(u, v, projection);
                // Accumulate the pixel color from samples
                //pixel_color += traceRayPhong(ray, background, world);
                pixel_color += traceRayRecursive(ray, RGBColor(0, 0, 0), world, max_depth);
            }
            // Divide the accumulated color by the number of accumulations
            pixel_color = pixel_color / samples_per_pixel;

            /**
             * Get the pixel in the PNG and set it to the right color.
             * Note (0,0) is the upper-left corner of the png file, but the
             * lower-left corner of the renderer's coordinates, so we have to
             * flip the y-coordinate
             */
            RGBAPixel& pixel = render->getPixel(x, image_height - 1 - y);
            pixel.setColor(pixel_color);
        }
    }

    render->writeToFile("renders/" + render_name + ".png");
    delete render;

    std::cout << "\nDone!\n";

    return 0;
}
