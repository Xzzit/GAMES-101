# Assignment 3

## Overview
In this programming task, we will further simulate modern graphics techniques. We have added an `Object Loader` (for loading 3D models), a `Vertex Shader`, and a `Fragment Shader` to the code, and we have also implemented support for texture mapping.

## Assignment
1. Modify the function `rasterize_triangle(const Triangle& t)` in `rasterizer.cpp`: Implement an interpolation algorithm similar to that in Assignment 2 here, to interpolate normals, colors, and texture colors.
2. Modify the function `get_projection_matrix()` in `main.cpp`: Fill in your own implementation of the projection matrix from previous experiments here. After this, you can run `./Rasterizer output.png normal` to observe the implementation results of normals.
3. Modify the function `phong_fragment_shader()` in `main.cpp`: Implement the calculation of Fragment Color using the Blinn-Phong model.
4. Modify the function `texture_fragment_shader()` in `main.cpp`: On the basis of implementing Blinn-Phong, consider the texture color as kd in the formula to implement the Texture Shading Fragment Shader.
5. Modify the function `bump_fragment_shader()` in `main.cpp`: On the basis of implementing Blinn-Phong, carefully read the comments in the function to implement Bump mapping.
6. Modify the function `displacement_fragment_shader()` in `main.cpp`: On the basis of implementing Bump mapping, implement displacement mapping.

## Explanation
We have made the following modifications to the framework compared to the last experiment:
1. We have introduced a third-party .obj file loading library to read more complex model files. This part of the library is in the `OBJ_Loader.h` file. You do not need to understand its working principle in detail; just know that this library will pass us a `Vector` named `TriangleList`, where each triangle has corresponding vertex normals and texture coordinates. Additionally, textures related to the model will also be loaded. Note: If you want to try loading other models, you can currently only manually change the model path.
2. We have introduced a new Texture class to generate textures from images and provided an interface to look up texture colors: `Vector3f getColor(float u, float v)`.
3. We created a `Shader.hpp` header file and defined `fragment_shader_payload`, which includes parameters that may be used by the `Fragment Shader`. Currently, there are three Fragment Shaders in `main.cpp`, where `fragment_shader` is an example Shader that colors according to the normals. The remaining two will be implemented by you.
4. The main rendering pipeline begins with `rasterizer::draw(std::vector<Triangle> &TriangleList)`. We perform a series of transformations again, typically completed by the `Vertex Shader`. After this, we call the `rasterize_triangle` function.
5. The `rasterize_triangle` function is similar to what you implemented in Assignment 2. The difference is that the set values will no longer be constants, but instead, they will interpolate normals, colors, texture colors, and Shading Colors using Barycentric Coordinates. Recall the [alpha, beta, gamma] we provided last time for computing the z value; this time you will need to apply it to the interpolation of other parameters. What you need to do is compute the interpolated color and write the color calculated by the Fragment Shader into the framebuffer. This requires you to first set the fragment shader payload with the interpolated results and call the fragment shader to get the computed outcome.

## Grading
* [5 points] Correct submission format, including all required files. Code should compile and run normally.
* [10 points] Parameter Interpolation: Correctly interpolate colors, normals, texture coordinates, and positions (Shading Position) and pass them to fragment_shader_payload.
* [20 points] Blinn-Phong Reflection Model: Correctly implement the reflection model corresponding to phong_fragment_shader.
* [5 points] Texture Mapping: Copy the code of phong_fragment_shader into texture_fragment_shader, and correctly implement Texture Mapping on this basis.
* [10 points] Bump Mapping and Displacement Mapping: Correctly implement Bump Mapping and Displacement Mapping.
* [Bonus 3 points] Try More Models: Find other usable .obj files, submit the rendering results, and save the models in the /models directory. These models should also contain Vertex Normal information.
* [Bonus 5 points] Bilinear Texture Interpolation: Use bilinear interpolation for texture sampling. Implement a new method Vector3f getColorBilinear(float u, float v) in the Texture class and call it through the fragment shader. To make the effects of bilinear interpolation more apparent, you should consider choosing smaller texture images. Please submit both the results of texture interpolation and bilinear texture interpolation, and compare them.

## Compile and Run
```
mkdir build
cd build
cmake ..
make
```

```
./Rasterizer output.png normal          // Use the normal shader.

./Rasterizer output.png phong           // Use the phong shader.

./Rasterizer output.png texture         // Use the texture shader.

./Rasterizer output.png bump            // Use the bump shader.

./Rasterizer output.png displacement    // Use the displacement shader.
```