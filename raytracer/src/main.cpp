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
 * The API here looks a little weird, because I built the recursive version
 * of the function first according to Peter Shirley's book. And I didn't want
 * to implement an extra function to get the albedo of a material.
 * @param ray The ray to trace.
 * @param background The background color.
 * @param world The scene to render and check for ray intersections.
 * @return The RGBColor to use to color the pixel.
 */
RGBColor traceRay(const Ray & ray, const RGBColor & background,
                  const Hittable & world) {
    // Put a point light source at (8, 8, 8)
    Point3 light_source(3, 8, 8);
    // Record what the ray hits
    hit_record record;
    // Diffuse constant
    double k_diffuse = 1.2;
    // Ambient light amount
    RGBColor ambient(1.0, 0.9, 0.6);
    // Ambient constant
    double k_ambient = 0.4;

    // If the ray hit something, do basic diffuse lighting
    if (world.hit(ray, 0.001, infinity, record)) {
        RGBColor ambient_color;
        Ray scattered;
        record.material->scatter(ray, record, ambient_color, scattered);
        
        Vec3 light_direction = light_source - record.point;
        auto light_angle = Vec3::dot(Vec3::normalize(light_direction),
                                     Vec3::normalize(record.normal));
        
        // If there's something blocking the light, make a hard shadow
        Ray shadow_ray = Ray(record.point, light_direction);
        RGBColor diffuse_color = ambient_color;
        auto distance_to_light = light_direction.length();
        if (world.hit(shadow_ray, 0.001, distance_to_light, record)) {
            diffuse_color *= 0.3;
        }
        
        return (k_ambient * ambient_color * ambient) +
            (k_diffuse * light_angle * diffuse_color);
    }

    // If the ray did not hit anything, color the sky a gradient
    Vec3 direction_normal = Vec3::normalize(ray.direction());
    auto t = 0.5 * (direction_normal.y() + 1.0);
    return (1.0-t) * RGBColor(1.0, 1.0, 1.0) + t * RGBColor(0.4, 0.6, 1.0);
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
    return (1.0-t) * RGBColor(1.0, 1.0, 1.0) + t * RGBColor(0.4, 0.6, 1.0);
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
void multiJitter(Point2 p[], unsigned cols, unsigned rows) {
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
    const auto aspect_ratio = 16.0 / 9.0;
    const unsigned int image_width = 640;
    const unsigned int image_height = 
        static_cast<unsigned int>(image_width / aspect_ratio);
    const unsigned int samples_per_pixel = 64;  // MUST BE A SQUARE NUMBER
    const int max_depth = 50;
    PNG *render = new PNG(image_width, image_height);
    RGBColor background(0, 0, 0);

    // Set up world
    HittableList world = basicScene();

    // Set up camera
    Point3 camera_pos = Point3(12, 2, 5);
    Point3 lookat = Point3(0, 1, -0.4);
    Vec3 up(0, 1, 0);
    auto fov = 20.0;
    auto focal_distance = 10.0;
    auto aperture = 0.1;
    RRTenum projection = RRTenum::PERSPECTIVE;
    
    Camera cam(camera_pos, lookat, up, fov, aspect_ratio, aperture, focal_distance);

    // Render the image!
    // Loop over each row of pixels (scanline)
    for (unsigned y = 0; y < image_height; ++y) {
        // Show the progress on the console
        std::cout << "\rScanlines remaining: " << image_height - y << ' ' << std::flush;
        
        // For each pixel in the row
        for (int x = 0; x < image_width; ++x) {
    
            // Set up a multi-jitter sample pattern
            auto sample_pattern_rows = std::sqrt(samples_per_pixel);
            Point2 sample_pattern[samples_per_pixel];
            multiJitter(sample_pattern, sample_pattern_rows, sample_pattern_rows);

            // Get the pixel color, with antialiasing
            RGBColor pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (x + sample_pattern[s].x) / (image_width - 1);
                auto v = (y + sample_pattern[s].y) / (image_height - 1);
                Ray ray = cam.getRay(u, v, projection);
                // Accumulate the pixel color from samples
                pixel_color += traceRay(ray, background, world);
            }
            // Divide the accumulated color by the number of accumulations
            pixel_color = pixel_color / samples_per_pixel;

            // // Get the pixel color, no antialiasing!
            // RGBColor pixel_color(0, 0, 0);
            // auto u = (x + 0.5) / (image_width - 1);
            // auto v = (y + 0.5) / (image_height - 1);
            // Ray ray = cam.getRay(u, v, projection);
            // // Accumulate the pixel color from samples
            // pixel_color += traceRay(ray, background, world);

            /**
             * Get the pixel in the PNG and set it to the right color.
             * Note (0,0) is the upper-left corner of the png file, but the
             * lower-left corner of the renderer's coordinates, so we have to
             * flip the y-coordinate
             */
            RGBAPixel &pixel = render->getPixel(x, image_height - 1 - y);
            pixel.setColor(pixel_color);
        }
    }

    render->writeToFile("renders/" + render_name + ".png");
    delete render;

    std::cout << "\nDone!\n";

    return 0;
}
