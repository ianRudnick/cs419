# BVH Stress Testing #

| Number of Spheres | Render Time - BVH (not including BVH construction )| Render Time - BVH (including BVH construction) | Render Time - No BVH | BVH Speedup Factor |
|--------|------|--------|---------|-----|
| 1,000  | 1s   | 1s     | 1m 16s  | 76x
| 10,000 | 6s   | 10s    | 14m 22s | 86x
| 100,000| 46s  | 8m 41s | 2h 27m (estimated) | 17x

My renderer is written in C++. I ran these stress tests on an AMD Ryzen 5 3600XT CPU (clock speed ~4300 MHz). I used nine samples per pixel antialiasing.

The time to construct the BVH tree for 100,000 spheres clearly doesn't scale well. I believe this is because I put k=1 sphere primitives at each leaf node, and I could speed up the construction for the future by going with k=2 or k=3 primitives per leaf.

Similarly, the dragon.obj model took about eight minutes to set up a BVHTree, but it also took a lot longer to render the model after that (about ten minutes). This suggests that either the BVHTree that was built for the dragon was not very good, or that my triangle hit function is much slower than the sphere hit function. I think the main issue is that my method for dealing with failure of the midpoint partitioning scheme isn't very good (randomly split the nodes in half). So if I increase k this will probably improve too.