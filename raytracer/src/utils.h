/**
 * @file utils.h
 * @author Ian Rudnick
 * Common constants and utilities for the raytracer.
 */

#ifndef RUDNICKRT_UTILS_H
#define RUDNICKRT_UTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

namespace rudnick_rt {

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897532385;


// Utility functions

/**
 * Converts from degrees to radians.
 * @param degrees The degree amount to convert.
 * @return The amount in radians.
 */
inline double degToRad(double degrees) {
    return degrees * pi / 180.0;
}

/**
 * Generates a pseudo-random number.
 * @return A random real number in [0, 1).
 */
inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/**
 * Generates a pseudo-random number between two numbers.
 * @param min The lower bound.
 * @param max The upper bound.
 * @return A random real number in [min, max).
 */
inline double randomDouble(double min, double max) {
    return min + (max-min) * randomDouble();
}

/**
 * Generates a pseudo-random integer between two integers.
 * @param min The lower bound.
 * @param max The upper bound.
 * @return A random integer in [min, max].
 */
inline int randomInt(int min, int max) {
    return static_cast<int>(randomDouble(min, max+1));
}

/**
 * Clamps a value to a range if it is outside of that range.
 * @param x The value to clamp.
 * @param min The minimum value to allow x to be.
 * @param max The maximum value to allow x to be.
 */
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

} // namespace rudnick_rt

#endif