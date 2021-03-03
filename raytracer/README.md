**CS 419 MP1 Raytracer**
by Ian Rudnick

This project is written in C++. A GNU Makefile is included to compile the code
with clang++. To compile, navigate to this directory in your linux terminal and
type "make".

To run the renderer, type ./main in your linux terminal.
(Sorry I didn't feel like compiling for Windows)

-------------------------------------------------------------------------------

All of the source code is located in the "src" folder.

To configure the renderer, modify main.cpp (specifically, most of the
important parameters are in the main() function)

**Planes** are in aa_rectangle.h and aa_rectangle.cpp

**Spheres** are in sphere.h and sphere.cpp

**Triangles** are in triangle.h and triangle.cpp

**Movable Camera** is in camera.h and camera.cpp

**Projections** are part of the camera functionality

**Multi-Jittered Sampling** is in main.cpp (look at lines 165 and 250)

**Diffuse Shading** is done in main.cpp function traceRay()

**Hard Shadows** also done in traceRay()

-------------------------------------------------------------------------------

This renderer outputs png images into the "renders" folder. I have included all
the scenes I rendered in there.
