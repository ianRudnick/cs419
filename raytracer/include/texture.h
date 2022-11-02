/**
 * @file texture.h
 * @author Ian Rudnick
 * Classes for procedural textures.
 */
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <memory>
#include <string>

#include "perlin.h"
#include "vec3.h"
#include "stb_image/stb_image_include.h"

using std::shared_ptr;
using std::make_shared;

namespace rudnick_rt {
/**
 * Abstract class for textures.
 * Derived classes must implement a way to get the texture color value at a
 * specific point.
 */
class Texture {
public:
	virtual RGBColor value(double u, double v, const Point3& p) const = 0;
};


//-----------------------------------------------------------------------------
/**
 * Class for basic solid-color textures.
 */
class SolidColorTexture : public Texture {
public:
	virtual ~SolidColorTexture() = default;

	/**
	 * Constructs a default solid color texture.
	 * Sets the texture color to white.
	 */
	SolidColorTexture() : color_value_(RGBColor(1, 1, 1)) {}

	/**
	 * Constructs a solid color texture with the given color.
	 * @param c The color for the texture.
	 */
	SolidColorTexture(RGBColor c) : color_value_(c) {}

	/**
	 * Constructs a solid color texture with the given color.
	 * @param red The red value, in range [0, 1].
	 * @param green The green value, in range [0, 1].
	 * @param blue The blue value, in range [0, 1].
	 */
	SolidColorTexture(float red, float green, float blue)
		: SolidColorTexture(RGBColor(red, green, blue)) {}

	/**
	 * Gets the color value of the texture at a specific point.
	 * @return The color value of the texture.
	 */
	virtual RGBColor value(double u, double v, const Vec3& p) const override {
		return this->color_value_;
	}

private:
	RGBColor color_value_;
};


//-----------------------------------------------------------------------------
/**
 * Class for a square checkerboard texture with two colors.
 */
class CheckerTexture : public Texture {
public:
	virtual ~CheckerTexture() = default;
	CheckerTexture() {}

	CheckerTexture(shared_ptr<Texture> even, shared_ptr<Texture> odd)
		: even_(even), odd_(odd) {}

	CheckerTexture(RGBColor even, RGBColor odd)
		: even_(make_shared<SolidColorTexture>(even)),
		  odd_(make_shared<SolidColorTexture>(odd)) {}

	virtual RGBColor value(double u, double v, const Point3& p) const override {
		auto sin_pattern = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
		if (sin_pattern < 0) {
			return this->odd_->value(u, v, p);
		}
		else {
			return this->even_->value(u, v, p);
		}
	}

private:
	shared_ptr<Texture> even_, odd_;
};


//-----------------------------------------------------------------------------
class PerlinTexture : public Texture {
public:
	virtual ~PerlinTexture() = default;
	PerlinTexture() {}
	PerlinTexture(double sc) : scale_(sc) {}

	virtual RGBColor value(double u, double v, const Point3& p) const override {
		// Cast the Perlin values between 0 and 1
		// return color(1, 1, 1) * 0.5 * (1.0 + this->noise_.noise(this->scale_ * p));

		// return color(1, 1, 1) * this->noise_.turbulence(this->scale_ * p);

		return RGBColor(1, 1, 1) * 0.5 * (1 + sin(this->scale_*p.z() + 50*this->noise_.turbulence(p)));
	}

public:
	Perlin noise_;
	double scale_;
};


//-----------------------------------------------------------------------------
class ImageTexture : public Texture {
public:
	const static int bytes_per_pixel_ = 3;

	/**
	 * Constructs an empty image texture object.
	 */
	ImageTexture()
		: data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

	/**
	 * Constructs an image texture from a given image file.
	 */
	ImageTexture(const std::string& filename) {
		auto channels = bytes_per_pixel_;

		this->data_ =
			stbi_load(filename.c_str(), &this->width_, &this->height_, &channels, 0);

		if (!this->data_) {
			std::cerr << "Error loading texture image file '" << filename << "'.\n";
			this->width_ = this->height_ = 0;
		}

		this->bytes_per_scanline_ = bytes_per_pixel_ * this->width_;
	}

	/**
	 * Frees the image data array.
	 */
	virtual ~ImageTexture() {
		stbi_image_free(this->data_);
	}

	/**
	 * Gets the color value at a certain point in the image texture.
	 */
	virtual RGBColor value(double u, double v, const Point3& p) const override {
		// If the texture data is empty/broken, return cyan to help debug
		if (this->data_ == nullptr) {
			return RGBColor(0, 1, 1);
		}

		// Clamp input texcoords to [0, 1]
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // flip v, because image files are upside down

		auto i = static_cast<int>(u * this->width_);
		auto j = static_cast<int>(v * this->height_);

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= this->width_) i = this->width_ - 1;
		if (j >= this->height_) j = this->height_ - 1;

		const auto color_scale = 1.0/255.0;
		auto pixel = this->data_ + j*this->bytes_per_scanline_ + i*this->bytes_per_pixel_;

		return RGBColor(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
	}

private:
	unsigned char* data_;
	int width_, height_;
	int bytes_per_scanline_;
};

} // namespace rudnick_rt

#endif