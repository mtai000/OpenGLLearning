#pragma once

#include "Utils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Base.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Chapter4_3 : public ChapterBase {
private:
	void setupVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);

	float cameraX, cameraY, cameraZ;
	float cubeLocX, cubeLocY, cubeLocZ;
	float pyrLocX, pyrLocY, pyrLocZ;

	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	GLuint mvLoc, projLoc, vLoc, mLoc, tfLoc;
	int width, height;
	float aspect, timeFactor;

	glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
public:
	Chapter4_3();
	~Chapter4_3();
	int run(void);

};