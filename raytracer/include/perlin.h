/**
 * @file perlin.h
 * @author Ian Rudnick
 * Class for creating Perlin noise for textures.
 */
#ifndef PERLIN_H
#define PERLIN_H

#include <cmath>

#include "utils.h"
#include "vec3.h"

namespace rudnick_rt {

class Perlin {
public:
	/**
	 * Constructs a randomized Perlin noise pattern.
	 */
	Perlin() {
		this->rand_vecs_ = new Vec3[num_points_];
		for (int i = 0; i < num_points_; ++i) {
			this->rand_vecs_[i] = Vec3::normalize(Vec3::randomInUnitSphere());
		}

		this->perm_x_ = perlinGeneratePerm();
		this->perm_y_ = perlinGeneratePerm();
		this->perm_z_ = perlinGeneratePerm();
	}

	/**
	 * Frees the arrays used in the Perlin object.
	 */
	~Perlin() {
		delete[] this->rand_vecs_;
		delete[] this->perm_x_;
		delete[] this->perm_y_;
		delete[] this->perm_z_;
	}

	/**
	 * Gets Perlin noise for a given point in space.
	 */
	double noise(const Point3& p) const {
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());

		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		Vec3 c[2][2][2];

		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = this->rand_vecs_[
						this->perm_x_[(i+di) & 255] ^
						this->perm_y_[(j+dj) & 255] ^
						this->perm_z_[(k+dk) & 255]
					];
				}
			}
		}

		return perlinInterp(c, u, v, w);
	}

	/**
	 * Gets Perlin noise with turbulence.
	 */
	double turbulence(const Point3& p, int depth = 7) const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; ++i) {
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}

		return fabs(accum);
	}

private:
	static const int num_points_ = 256;
	Vec3* rand_vecs_;
	int* perm_x_;
	int* perm_y_;
	int* perm_z_;

	/**
	 * Generates a Perlin noise pattern for one dimension.
	 */
	static int* perlinGeneratePerm() {
		auto p = new int[Perlin::num_points_];

		for (int i = 0; i < Perlin::num_points_; ++i) {
			p[i] = i;
		}

		permute(p, Perlin::num_points_);
		return p;
	}

	/**
	 * Swaps each element in the array p with a random one, creating the base
	 * noise pattern.
	 */
	static void permute(int* p, int n) {
		for (int i = n-1; i > 0; i--) {
			int target = randomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	/**
	 * Linearly interpolates using trilinear interplation.
	 * @param c The data to interpolate.
	 * @param u The amount to interpolate in the first dimension.
	 * @param v The amount to interpolate in the second dimension.
	 * @param w The amount to interpolate in the third dimension.
	 */
	static double perlinInterp(Vec3 c[2][2][2], double u, double v, double w) {
		// Use a Hermite cubic to round off the interpolation
		auto uu = u*u*(3-2*u);
		auto vv = v*v*(3-2*v);
		auto ww = w*w*(3-2*w);
		auto accum =  0.0;

		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					Vec3 weight_v(u-i, v-j, w-k);

					accum += (i*uu + (1-i) * (1-uu)) *
							 (j*vv + (1-j) * (1-vv)) *
							 (k*ww + (1-k) * (1-ww)) *
							 Vec3::dot(c[i][j][k], weight_v);
				}
			}
		}
		return accum;
	}
};

} // namespace rudnick_rt
#endif