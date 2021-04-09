# CS 419 MP3 Raytracer #
**by Ian Rudnick**

This project is written in C++. A GNU Makefile is included to compile the code with clang++. To compile, navigate to this directory in your linux terminal and type "make".

To run the renderer, type ./main in your linux terminal.

-------------------------------------------------------------------------------
## Source Code ##
All of the source code is located in the "src" folder.

To configure the renderer, modify main.cpp (specifically, most of the important parameters are in the main() function)

-------------------------------------------------------------------------------
## Code From Other Sources ##
tiny_obj_loader.h: https://github.com/tinyobjloader/tinyobjloader - for loading obj files

lodepng.h / lodepng.cpp: https://github.com/lvandeve/lodepng - for encoding png files

Most of the rendering engine structure is based on Peter Shirley's Ray Tracing In One Weekend, but I typed it all out myself, documented it reasonably well, and followed my regular style conventions.

I engineered most of the BVH tree and triangle mesh implementations myself. BVH midpoint algorithm was based off the source linked in the MP2 spec.

-------------------------------------------------------------------------------
## Renders ##
This renderer outputs png images into the "renders" folder. I have included all the scenes I rendered in there.

**area_light.png** shows my rectangular area light, as well as reflective and transparent spheres.

**materials.png** shows three 3D models using different materials. The left cow has the reflective material, with some degree of "fuzz", which adds a random component to the ray scatter direction. The right cow is a dielectric with refraction coefficient of 1.5, which is about that of glass. The floor also has the reflective material, with no "fuzz".

**my_apartment.png** is a completely accurate photorealistic rendering of my real apartment. Actually it's just a cow and some marbles in a box, but it uses mirror reflection, transparency, and area lights, and looks pretty cool.

-------------------------------------------------------------------------------
## MP3 ##
All features for this assignment are implemented in **materials.h** and **materials.cpp**. I also added a modified ray-trace function in **main.cpp** to allow the user to specify the background color, instead of using the gradient sky background used in RTiOW. Usually I think you want to set this to pitch-black (0, 0, 0), and just use a bunch of area lights in the scene.
