/**
 * @file vec3.cpp
 * @author Ian Rudnick
 * Implementation of a 3-component vector class for use in raytracing
 * calculations.
 */

#include <cmath>
#include "vec3.h"


namespace rudnick_rt {

Vec3::Vec3() : v_{0, 0, 0} {}
Vec3::Vec3(double x, double y, double z) : v_{x, y, z} {}

double Vec3::x() const { return v_[0]; }
double Vec3::y() const { return v_[1]; }
double Vec3::z() const { return v_[2]; }

double Vec3::operator[](unsigned i) const { return v_[i]; }
double & Vec3::operator[](unsigned i) { return v_[i]; }

//-----------------------------------------------------------------------------
// Methods

void Vec3::set(double x, double y, double z) {
    v_[0] = x;
    v_[1] = y;
    v_[2] = z;
}

double Vec3::length() const {
    return std::sqrt(lengthSquared());
}

double Vec3::lengthSquared() const {
    return v_[0]*v_[0] + v_[1]*v_[1] + v_[2]*v_[2];
}

double Vec3::dot(const Vec3 & a, const Vec3 & b) {
    return a.v_[0] * b.v_[0]
         + a.v_[1] * b.v_[1]
         + a.v_[2] * b.v_[2];
}

Vec3 Vec3::cross(const Vec3 & a, const Vec3 & b) {
    return Vec3(a.v_[1] * b.v_[2] - a.v_[2] * b.v_[1],
                a.v_[2] * b.v_[0] - a.v_[0] * b.v_[2],
                a.v_[0] * b.v_[1] - a.v_[1] * b.v_[0]);
}

Vec3 Vec3::normalize(Vec3 vector) {
    return vector / vector.length();
}

Vec3 Vec3::random() {
    return Vec3(randomDouble(), randomDouble(), randomDouble());
}

Vec3 Vec3::random(double min, double max) {
    return Vec3(randomDouble(min, max),
                randomDouble(min, max),
                randomDouble(min, max));
}

/**
 * Gets a random vector on or in the unit sphere by getting a random vector
 * with components in [-1, 1], and trying again if it's not on or in the unit
 * sphere.
 */
Vec3 Vec3::randomInUnitSphere() {
    Vec3 out;
    do {
        out = Vec3::random(-1, 1);
    } while (out.lengthSquared() > 1);
    
    //Vec3::normalize(random);
    return out;
}

Vec3 Vec3::randomInHemisphere(const Vec3 & normal) {
    Vec3 random = randomInUnitSphere();
    Vec3::normalize(random);

    if (Vec3::dot(random, normal) > 0.0) {
        return random;
    }
    else {
        return -random;
    }
}

Vec3 Vec3::randomInUnitDisc() {
    Vec3 out;
    do {
        out = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
    } while (out.lengthSquared() > 1);

    return out;
}

bool Vec3::nearZero() const {
    const auto s = 1e-8;
    return (fabs(v_[0]) < s) && (fabs(v_[1]) < s) && (fabs(v_[2]) < s);
}

Vec3 Vec3::reflect(const Vec3 & incident, const Vec3 & normal) {
    return incident - (2 * Vec3::dot(incident, normal) * normal);
}

Vec3 Vec3::refract(const Vec3 & incident, const Vec3 & normal,
                   double index_ratio) {

    // Calculate the cosine between the incident and normal vectors
    auto cos_theta = fmin(Vec3::dot(-incident, normal), 1.0);

    // Calculate the components of the output ray
    Vec3 perpendicular = index_ratio * (incident + cos_theta * normal);
    Vec3 parallel = -std::sqrt(fabs(1.0-perpendicular.lengthSquared()))*normal;

    // Combine the two components
    return perpendicular + parallel;
}


//-----------------------------------------------------------------------------
// Operators

Vec3 Vec3::operator-() const {
    return Vec3(-v_[0], -v_[1], -v_[2]);
}

Vec3 & Vec3::operator+=(const Vec3 & other) {
    v_[0] += other.v_[0];
    v_[1] += other.v_[1];
    v_[2] += other.v_[2];
    return *this;
}

Vec3 & Vec3::operator*=(const double scalar) {
    v_[0] *= scalar;
    v_[1] *= scalar;
    v_[2] *= scalar;
    return *this;
}

Vec3 & Vec3::operator/=(const double scalar) {
    return *this *= 1.0/scalar;
}

Vec3 operator+(const Vec3 & a, const Vec3 & b) {
    return Vec3(a.v_[0] + b.v_[0],
                a.v_[1] + b.v_[1],
                a.v_[2] + b.v_[2]);
}

Vec3 operator-(const Vec3 & a, const Vec3 & b) {
    return Vec3(a.v_[0] - b.v_[0],
                a.v_[1] - b.v_[1],
                a.v_[2] - b.v_[2]);
}

Vec3 operator*(const Vec3 & a, const Vec3 & b) {
    return Vec3(a.v_[0] * b.v_[0],
                a.v_[1] * b.v_[1],
                a.v_[2] * b.v_[2]);
}

Vec3 operator*(const Vec3 & vector, double scalar) {
    return Vec3(scalar * vector.v_[0], 
                scalar * vector.v_[1], 
                scalar * vector.v_[2]);
}

Vec3 operator*(double scalar, const Vec3 & vector) {
    return Vec3(scalar * vector.v_[0], 
                scalar * vector.v_[1], 
                scalar * vector.v_[2]);
}

Vec3 operator/(const Vec3 & vector, double scalar) {
    return vector * (1.0/scalar);
}

} // namespace rudnick_rt
