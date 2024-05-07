#pragma once
#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>


class  Lesson01
{
public:
	Lesson01();
	~Lesson01();
	static int run(int argc, const char** argv);
private:
	static Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
	static Eigen::Matrix4f get_model_matrix(float rotation_angle);
	static Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);
};
