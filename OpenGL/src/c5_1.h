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
#include <SOIL2/SOIL2.h>

class Chapter5_1 : public ChapterBase {
private:
	void setupVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);

	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];
	GLuint mvLoc, projLoc;

	float cameraX, cameraY,cameraZ;
	float pyrLocX, pyrLocY, pyrLocZ;
	int width, height;
	glm::mat4 pMat, vMat, mMat, mvMat;

	float aspect;
	GLuint brickTexture;

public:
	Chapter5_1();
	~Chapter5_1();
	int run(void);

};