// OpenGL.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Utils.h"
#include "Base.h"

using namespace std;
// TODO: Reference additional headers your program requires here.


class Chapter2 : public ChapterBase {
public:
	int run(void);

private:

#define numVAOs 1
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	Utils* utils = new Utils();

	float x = 0.0f;
	float step = 0.01f;

	void init(GLFWwindow* windows);
	void display(GLFWwindow* window, double currentTime);


};