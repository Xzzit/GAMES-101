# Assignment 7

## Overview
In our previous exercises, we implemented the Whitted-Style Ray Tracing algorithm and accelerated the intersection process using structures like BVH (Bounding Volume Hierarchy). In this experiment, we will build upon our last experiment to implement a complete Path Tracing algorithm. With this, we arrive at the final section of the ray tracing module.

Please read this document carefully and follow the instructions outlined herein to complete this experiment.

## Code Modification
We have modified the following content in the code framework:

* Modified `main.cpp` to accommodate the test model CornellBox for this experiment.

* Updated Render to adapt to CornellBox and support multiple samples per pixel as required by Path Tracing.

* Altered Object, Sphere, Triangle, TriangleMesh, and BVH by adding an area attribute and a Sample method. This is to implement area sampling of light sources and adding an interface sampleLight in Scene for sampling light sources.

* Revised Material and implemented three methods: sample, eval, and pdf for auxiliary calculations of Path Tracing variables.

## Content You Need to Migrate
You need to directly copy the following functions from your previous programming exercise to the corresponding locations:

* `Triangle::getIntersection` in `Triangle.hpp`: Paste your ray-triangle intersection function here. Please directly copy the content implemented in the last experiment to this place.

* `IntersectP(const Ray& ray, const Vector3f& invDir, const std::array<int, 3>& dirIsNeg)` in `Bounds3.hpp`: The purpose of this function is to determine whether the bounding box (BoundingBox) intersects with the ray. Please directly paste the content implemented in the last experiment here, and make sure to check if the judgment is correct when t_enter = t_exit.

* `getIntersection(BVHBuildNode* node, const Ray ray)` in `BVH.cpp`: This is for the BVH search process. Please directly paste the content implemented in the last experiment here.

## Code Implementation
In this experiment, you only need to modify one function:

* `castRay(const Ray ray, int depth)` in `Scene.cpp`: Implement the Path Tracing algorithm here.

Functions that may be used include:

* `intersect(const Ray ray)` in `Scene.cpp`: Find the intersection of a ray with the scene.

* `sampleLight(Intersection pos, float pdf)` in `Scene.cpp`: Uniformly sample a point on all light sources in the scene by area, and calculate the probability density of that sample.

* `sample(const Vector3f wi, const Vector3f N)` in `Material.cpp`: Given the incident direction and normal vector, sample an outgoing direction according to the distribution dictated by the material's properties.

* `pdf(const Vector3f wi, const Vector3f wo, const Vector3f N)` in `Material.cpp`: Given a pair of incident and outgoing directions with a normal vector, calculate the probability density for the outgoing direction obtained by the sample method.

* `eval(const Vector3f wi, const Vector3f wo, const Vector3f N)` in `Material.cpp`: Given a pair of incident and outgoing directions with a normal vector, compute the value of f_r under these conditions.

Variables that may be used include:

* `RussianRoulette` in `Scene.cpp`: P_RR, the probability for Russian Roulette.

## Implementation Hints
The pseudocode for Path Tracing introduced in the course is as follows (to maintain consistency with the previous framework, the definition of wo is opposite to that introduced in the course):

```
shade(p, wo):
    Uniformly sample the light at xx (pdf_light = 1 / A)
    Shoot a ray from p to x
    If the ray is not blocked in the middle
        L_dir = L_i * f_r * cos_theta * cos_theta_x / |x-p|^2 / pdf_light
    
    L_indir = 0.0
    Test Russian Roulette with probability P_RR
    Uniformly sample the hemisphere toward wi (pdf_hemi = 1 / 2pi)
    Trace a ray r(p, wi)
    If ray r hits a non-emitting object at q
        L_indir = shade(q, wi) * f_r * cos_theta / pdf_hemi / P_RR
    
    Return L_dir + L_indir
```

Based on the framework provided for this experiment, we can further rewrite the pseudocode as:

```
shade(p, wo):
    sampleLight(inter, pdf_light)
    Get x, ws, NN, emit from inter
    Shoot a ray from p to x
    If the ray is not blocked in the middle
        L_dir = emit * eval(wo, ws, N) * dot(ws, N) * dot(ws, NN) / |x-p|^2 / pdf_light
    
    L_indir = 0.0
    Test Russian Roulette with probability RussianRoulette
    wi = sample(wo, N)
    Trace a ray r(p, wi)
    If ray r hits a non-emitting object at q
        L_indir = shade(q, wi) * eval(wo, wi, N) * dot(wi, N) / pdf(wo, wi, N) / RussianRoulette
    
    Return L_dir + L_indir
```

Please make sure you have a clear understanding of the implementation of Path Tracing before moving on to the next segment of the discussion.

## Important Considerations
1. The execution of the code in this experiment is very slow. It is recommended to adjust the scene size in `main.cpp` or the SPP (Samples Per Pixel) number in `Render.cpp` during debugging to speed up the execution. Additionally, implementing multithreading can further accelerate the computation.

2. Pay attention to numerical precision issues, especially when the pdf is close to zero, and when judging the boundary conditions of whether the light is blocked during sampleLight. These situations often lead to excessive noise in the rendering results or the appearance of black horizontal stripes.

3. If you strictly follow the above algorithm, you will notice that the light source area appears purely black in the rendering results. Please analyze the reason for this phenomenon and modify the Path Tracing algorithm to make the light source visible.

## Compile and Run
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./RayTracing
```

