# Assignment 6

## Overview
In previous programming exercise, we implemented a basic ray tracing algorithm, specifically focusing on light transmission and intersection calculations between rays and triangles. Our method to find intersections between rays and scene objects involved traversing all objects in the scene and determining whether the ray intersects with them. This approach yields good results when the number of objects in the scene is small. However, as the number of objects increases and the models become more complex, this method becomes highly inefficient. Therefore, we need an acceleration structure to speed up the intersection process. In this exercise, we focus on the object partitioning algorithm Bounding Volume Hierarchy (BVH). This exercise requires you to implement Ray-Bounding Volume intersection and BVH search.

First, you need to reference the following functions from your last programming exercise:

* `Render()` in Renderer.cpp: Paste your ray generation process here and update the format of the corresponding calls according to the new framework.

* `Triangle::getIntersection` in Triangle.hpp: Paste your ray-triangle intersection function here and update the format of the corresponding intersection information according to the new framework.
In this programming exercise, you need to implement the following functions:

* `IntersectP(const Ray& ray, const Vector3f& invDir, const std::array<int, 3>& dirIsNeg)` in Bounds3.hpp: The purpose of this function is to determine whether the BoundingBox intersects with a ray. You need to implement the intersection process according to the algorithm introduced in the course.

* `getIntersection(BVHBuildNode* node, the Ray ray)` in BVH.cpp: After constructing the BVH, we can use it to accelerate the intersection process. This process is recursive, and you will call your implemented `Bounds3::IntersectP` within it."

## Code Framework
We have modified the following content in the code framework:

* Material.hpp: We have separated material parameters into an individual class. Now, each object instance can have its own material.

* Intersection.hpp: This data structure contains information related to intersections.

* Ray.hpp: The Ray class, which includes the source, direction, transmission time 't', and range of a light ray.

* Bounds3.hpp: The Bounding Box class, where each bounding box can be described by two points, pMin and pMax (consider why this is the case). The Bounds3::Union function is used to merge two bounding boxes into a larger one. Like materials, each object instance in the scene has its own bounding box.

* BVH.hpp: The BVH acceleration class. The scene has an instance of BVHAccel. Starting from the root node, we can recursively construct the BVH of the scene from the list of objects.