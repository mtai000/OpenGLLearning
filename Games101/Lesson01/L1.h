#pragma once
#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
    #define LIBRARY extern "C" _declspec(dllexport) 
#else
    #define LIBRARY
#endif


//
//class Lesson1
//{
//public:
//	Lesson1();
//	~Lesson1();
//	static int run(int argc, const char** argv);
//private:
//
//};

LIBRARY int lesson01(int argc, const char** argv);