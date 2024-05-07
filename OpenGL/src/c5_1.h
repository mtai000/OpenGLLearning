#pragma once


#include "Utils.h"

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
