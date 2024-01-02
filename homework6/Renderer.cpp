//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Scene.hpp"
#include "Renderer.hpp"
#include <opencv2/opencv.hpp>


inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(-1, 5, 10);
    int m = 0;
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                      imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

            // normalize this direction
            Vector3f dir = normalize(Vector3f(x, y, -1));

            // cast ray into scene and return color
            framebuffer[m++] = scene.castRay(Ray(eye_pos, dir), 0);

        }
        UpdateProgress(j / (float)scene.height);
    }
    UpdateProgress(1.f);

    // save framebuffer to file
    cv::Mat image(scene.height, scene.width, CV_8UC3);
    for (int i = 0; i < scene.height * scene.width; ++i) {
        image.at<cv::Vec3b>(i / scene.width, i % scene.width)[2] = (uchar)(255 * clamp(0, 1, framebuffer[i].x));
        image.at<cv::Vec3b>(i / scene.width, i % scene.width)[1] = (uchar)(255 * clamp(0, 1, framebuffer[i].y));
        image.at<cv::Vec3b>(i / scene.width, i % scene.width)[0] = (uchar)(255 * clamp(0, 1, framebuffer[i].z));
    }
    
    cv::imwrite("image.png", image); // Save as PNG   
}