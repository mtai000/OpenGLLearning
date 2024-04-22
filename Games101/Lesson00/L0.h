#pragma once

#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>

//class  Lesson0
//{
//public:
//	Lesson0();
//	~Lesson0();
//	static int run();
//private:
//
//};
#ifdef _WIN32
    #define LIBRARY extern "C" _declspec(dllexport) 
#else
    #define LIBRARY
#endif


LIBRARY int run(int argc, const char** argv);
int lesson00(int argc, const char** argv);
