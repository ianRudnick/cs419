/**
 * @file vec3.h
 * @author Ian Rudnick
 * 3-component vector class for use in raytracing calculations.
 * Based on Peter Shirley's Implementation in Ray Tracing In One Weekend.
 */

#ifndef RUDNICKRT_VEC3_H
#define RUDNICKRT_VEC3_H

#include <iostream>
#include "utils.h"

namespace rudnick_rt {
class Vec3 {
public:

    /**
     * Constructs a default Vec3 object. Initializes xyz to 0.
     */
    Vec3();

    /**
     * Constructs a Vec3 with the three specified dimensions.
     * @param x The x-component of the Vec3.
     * @param y The y-component of the Vec3.
     * @param z The z-component of the Vec3.
     */
    Vec3(double x, double y, double z);

    /** 
     * Vec3 data access function.
     * @return The x-component of the Vec3. 
     */
    double x() const;

    /** 
     * Vec3 data access function.
     * @return The y-component of the Vec3. 
     */
    double y() const;

    /** 
     * Vec3 data access function.
     * @return The z-component of the Vec3. 
     */
    double z() const;

    /**
     * Sets a Vec3's components to the given values.
     * @param x The value to set the x-component.
     * @param y The value to set the y-component.
     * @param z The value to set the z-component.
     */
    void set(double x, double y, double z);

    // Some straightforward operators:
    Vec3 operator-() const;

    double operator[](unsigned i) const;
    double & operator[](unsigned i);
    
    Vec3 & operator+=(const Vec3 & other);
    Vec3 & operator*=(const double scalar);
    Vec3 & operator/=(const double scalar);

    /**
     * Adds two Vec3s.
     * @param a The first Vec3 to add.
     * @param b The second Vec3 to add.
     * @return The sum, a + b.
     */
    friend Vec3 operator+(const Vec3 & a, const Vec3 & b);

    /**
     * Subtracts one Vec3 from another.
     * @param a The minuend.
     * @param b The subtrahend.
     * @return The difference, a - b.
     */
    friend Vec3 operator-(const Vec3 & a, const Vec3 & b);

    /**
     * Multiplies the components of two Vec3s.
     * @param a The first Vec3 to multiply.
     * @param b The second Vec3 to multiply.
     * @return Vec3 with components a[i] * b[i].
     */
    friend Vec3 operator*(const Vec3 & a, const Vec3 & b);

    /**
     * Multiplies a Vec3 by a scalar.
     * @param vector The Vec3 to multiply.
     * @param scalar The number to multiply each component by.
     * @return The product, vector * scalar.
     */
    friend Vec3 operator*(const Vec3 & vector, double scalar);

    /**
     * Multiplies a Vec3 by a scalar.
     * @param scalar The number to multiply each component by.
     * @param vector The Vec3 to multiply.
     * @return The product, scalar * vector.
     */
    friend Vec3 operator*(double scalar, const Vec3 & vector);

    /**
     * Divides a Vec3 by a scalar.
     * @param vector The Vec3 to divide.
     * @param scalar The number to divide each component by.
     * @return The quotient, vector / scalar.
     */
    friend Vec3 operator/(const Vec3 & vector, double scalar);

    /**
     * Calculates the scalar length of a Vec3.
     * @return The length of the Vec3.
     */
    double length() const;

    /**
     * Calculates the squared length of a Vec3.
     * @return The length of the Vec3, squared.
     */
    double lengthSquared() const;

    /**
     * Computes the dot product of two Vec3s.
     * @param a The first Vec3.
     * @param b The second Vec3.
     * @return The scalar dot product of the two Vec3s.
     */
    static double dot(const Vec3 & a, const Vec3 & b);
    
    /**
     * Computes the cross product of two Vec3s.
     * @param a The first Vec3.
     * @param b The second Vec3.
     * @return The vector cross product of the two Vec3s.
     */
    static Vec3 cross(const Vec3 & a, const Vec3 & b);

    /**
     * Normalizes a Vec3, making it unit-length.
     * @param vector The Vec3 to normalize
     * @return The normalized Vec3
     */
    static Vec3 normalize(Vec3 vector);

    /**
     * Get a random vector. This does not normalize the vector.
     * @return A Vec3 with random components in range [0, 1].
     */
    static Vec3 random();

    /**
     * Gets a random vector with a given range.
     * @param min Minimum possible value of each component.
     * @param max Maximum possible value of each component.
     * @return A Vec3 with random components in range [min, max].
     */
    static Vec3 random(double min, double max);

    /**
     * Gets a random vector within the unit circle.
     * @return A random vector on or in the unit circle.
     */
    static Vec3 randomInUnitSphere();

    /**
     * Gets a random vector on the same hemisphere as a given normal.
     * @param normal The normal to use.
     * @return A random normalized vector whose dot product with normal 
     * is positive.
     */
    static Vec3 randomInHemisphere(const Vec3 & normal);

    /**
     * Gets a random vector within a flat disc of radius 1.
     * @return A random vector on or in the unit disc.
     */
    static Vec3 randomInUnitDisc();

    /**
     * Checks if a vector is close to zero.
     * @return True if the vector is close to zero in all dimensions.
     */
    bool nearZero() const;

    /**
     * Reflects a vector off a surface defined by a normal vector.
     * @param incident The vector incident on the surface.
     * @param normal The normal vector of the surface.
     * @return The reflected vector.
     */
    static Vec3 reflect(const Vec3 & incident, const Vec3 & normal);

    /**
     * Refracts a vector through a surface defined by a normal vector.
     * @param incident The vector incident on the surface.
     * @param normal The normal vector of the surface.
     * @param index_ratio Ratio of the refraction indices (incident/surface)
     * @return The refracted vector.
     */
    static Vec3 refract(const Vec3 & incident, const Vec3 & normal, 
                        double index_ratio);


private:
    /* Array holding the vector components. */
    double v_[3];

}; // class Vec3


/**
 * Type aliases for Vec3, so that we don't risk mixing up points and colors
 */
using Point3 = Vec3;
using RGBColor = Vec3;

/**
 * Stream operator to allow Vec3s to be written to standard streams.
 * @param out Stream to write to.
 * @param v Vec3 to write to the stream.
 */
inline std::ostream & operator<<(std::ostream & out, const Vec3 & v) {
    return out << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')';
}

} // namespace rudnick_rt


#endif // VEC3_H
