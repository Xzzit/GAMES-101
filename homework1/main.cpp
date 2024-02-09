#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_model_matrix(
    float scale_fac,
    float rotation_angle,
    Eigen::Vector2f translation_vec
    )
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // Create the model matrix for scaling the triangle.
    Eigen::Matrix4f scale;
    scale << scale_fac, 0, 0, 0, 
             0, scale_fac, 0, 0, 
             0, 0, 1, 0, 
             0, 0, 0, 1;

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    Eigen::Matrix4f rotation;
    rotation << cos(rotation_angle/180 * MY_PI), -sin(rotation_angle/180 * MY_PI), 0, 0, 
                sin(rotation_angle/180 * MY_PI), cos(rotation_angle/180 * MY_PI), 0, 0, 
                0, 0, 1, 0, 
                0, 0, 0, 1;

    // Create the model matrix for translating the triangle.
    Eigen::Matrix4f translation;
    translation << 1, 0, 0, translation_vec.x(), 
                   0, 1, 0, translation_vec.y(), 
                   0, 0, 1, 0, 
                   0, 0, 0, 1;
    
    model = translation * rotation * scale * model;

    return model;
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate <<    1, 0, 0, -eye_pos[0], 
                    0, 1, 0, -eye_pos[1], 
                    0, 0, 1, -eye_pos[2], 
                    0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    Eigen::Matrix4f translate;
    translate <<    zNear / (aspect_ratio * zNear * tan(eye_fov / 360 * MY_PI)), 0, 0, 0,
                    0, zNear / (zNear * tan(eye_fov / 360 * MY_PI)), 0, 0,
                    0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
                    0, 0, -1, 0;

    projection = translate * projection;

    return projection;
}

int main(int argc, const char** argv)
{
    float scale = 1;
    float angle = 0;
    Eigen::Vector2f translation(0, 0);
    bool command_line = false;
    std::string filename = "output.png";

    // Check if command line arguments are provided
    if (argc >= 2) {
        command_line = true;

        // Parse command line arguments
        for (int i = 1; i < argc; i += 2) {
            std::string arg = argv[i];
            std::string value = argv[i + 1];

            if (arg == "-s") {
                scale = std::stof(value);
            } else if (arg == "-r") {
                angle = std::stof(value);
            } else if (arg == "-t") {
                float tx = std::stof(value);
                float ty = std::stof(argv[i + 2]);
                translation = Eigen::Vector2f(tx, ty);
                i++; // Skip the next argument since it has been processed
            } else if (arg == "-n") {
                filename = value;
            }
        }
    }

    // resize frame_buf and depth_buf vector length to w * h
    rst::rasterizer r(700, 700);

    // set eye position
    Eigen::Vector3f eye_pos = {0, 0, 5};

    // set triangle vertices position and indices
    std::vector<Eigen::Vector3f> pos{
        {0, 0, -2},
        {0, 2, -2}, {-2, 0, -2},
        {2, 0, -2}, {0, -2, -2}
        };

    std::vector<Eigen::Vector3i> ind{
        {0, 1, 2},
        {0, 3, 4}
        };

    // load to pos_buf and ind_buf
    auto pos_id = r.load_positions(pos); // pos_id.pos_id = 0
    auto ind_id = r.load_indices(ind); // ind_id.ind_id = 1

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        // clear frame_buf to (0, 0, 0) and depth_buf to infinity
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // set mvp matrix
        r.set_model(get_model_matrix(scale, angle, translation));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(scale, angle, translation));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'w') {
            translation.y() += 0.5;
        } else if (key == 's') {
            translation.y() -= 0.5;
        } else if (key == 'a') {
            translation.x() -= 0.5;
        } else if (key == 'd') {
            translation.x() += 0.5;
        } else if (key == 'q') {
            angle += 15;
        } else if (key == 'e') {
            angle -= 15;
        } else if (key == 'z') {
            scale += 0.2;
        } else if (key == 'x') {
            scale -= 0.2;
        }
    }

    return 0;
}
