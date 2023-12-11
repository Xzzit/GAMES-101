# Assignment 4

## Overview
Bézier curves are a type of parametric curve used in computer graphics. In this assignment, you are required to implement the de Casteljau algorithm to draw a Bézier curve represented by 4 control points (once you correctly implement this algorithm, you can support drawing Bézier curves controlled by more points).
The function you need to modify is in the provided main.cpp file.

* bezier: This function implements the drawing of the Bézier curve. It takes a sequence of control points and an OpenCV::Mat object as inputs, and does not return a value. It iterates t in the range of 0 to 1, increasing t by a small value each iteration. For each t that needs to be calculated, another function recursive_bezier is called, which then returns the point at t on the Bézier curve. Finally, the returned point is drawn on the OpenCV::Mat object.
* recursive_bezier: This function takes a sequence of control points and a floating-point number t as inputs, and implements the de Casteljau algorithm to return the coordinates of the corresponding point on the Bézier curve.

## Explanation
The de Casteljau algorithm is described as follows:

1. Consider a Bézier curve with a sequence of control points p0, p1, ... pn. First, connect adjacent points to form line segments.
2. Subdivide each line segment in the ratio of t : (1 − t) and find the corresponding division point.
3. The resulting division points will form a new sequence of control points, with the length of the sequence reduced by one.
4. If the sequence contains only one point, return that point and terminate. Otherwise, use the new sequence of control points and go back to step 1.

By executing the above algorithm with multiple different t values in the range [0,1], you will be able to generate the corresponding Bézier curve.

## Compile and Run
```
mkdir build
cd build
cmake ..
make
```