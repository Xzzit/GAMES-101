# Assignment 2
In our last assignment, although we managed to draw a wireframe triangle on the screen, it didn't look very interesting. Therefore, this time we are going to take it a step further - drawing a solid triangle on the screen, in other words, rasterizing a triangle. In the previous assignment, after the viewport transformation, we called the function `rasterize_wireframe(const Triangle& t)`. However, this time, you need to fill in and call the function `rasterize_triangle(const Triangle& t)` yourself.

The workflow inside this function is as follows:

* Create a 2D bounding box for the triangle.
* Traverse all the pixels within this bounding box (using their integer indices). Then, using the screen-space coordinates of the center of the pixels, check if the center point is inside the triangle.
* If it is inside, compare the interpolated depth value at its position with the corresponding value in the depth buffer.
* If the current point is closer to the camera, set the pixel color and update the depth buffer.

The functions you need to modify are as follows:

* `rasterize_triangle()`: Implements the triangle rasterization algorithm.
* `static bool insideTriangle()`: Tests if a point is inside a triangle. You can modify the definition of this function, which means you can update the return type or function parameters as you see fit.

Since we only know the depth values at the three vertices of the triangle, for pixels inside the triangle, we need to obtain their depth values through interpolation. We have already handled this part for you, as this content has not yet been covered in the course. The interpolated depth value is stored in the variable `z_interpolated`.

Please note how we initialize the depth buffer and pay attention to the sign of the z values. For ease of coding, we have reversed z to ensure they are all positive, with larger values indicating further from the viewpoint.

In this assignment, you do not need to deal with rotation transformations, just return an identity matrix for the model transformation.

You will notice that the `get_projection_matrix()` function in `main.cpp` is empty. Please copy and paste your implementation from the first assignment to fill in this function.

Finally, we provide two hard-coded triangles to test your implementation.

# Grading
* [5 points] Correct submission of all necessary files and code that compiles and runs.
* [20 points] Correct implementation of the triangle rasterization algorithm.
* [10 points] Correctly testing whether a point is inside a triangle.
* [10 points] Correct implementation of the z-buffer algorithm to draw triangles in the correct order on the screen.
* [Bonus 5 points] Implementing Anti-aliasing using super-sampling: You might notice that when we magnify the image, the edges of the image appear jagged. This issue can be addressed with super-sampling, i.e., sampling each pixel with a 2 * 2 grid and comparing the results (without the need to consider sample reuse between pixels). It is important to note that each sample within a pixel must maintain its own depth value, meaning each pixel should maintain a sample list. Lastly, if you implement this correctly, your triangles should not have abnormal black edges.

# Compile and Run
```
mkdir build
cd build
cmake ..
make
```

```
./Rasterizer            // Run the program in a loop, creating a window display

./Rasterizer image.png  // Run the program and save the result in image.png
```