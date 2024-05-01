#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>

using namespace std;

#define numVAOs 1
#define numVBOs 2

class  Utils
{
private:
	static GLuint prepareShader(int shaderType, const char* shaderPath);
	static string readShaderSource(const char* filepath);
	static void printShaderLog(GLuint shader);
	static void printProgramLog(int prog);
	static bool checkOpenGLError();
	static int finalizeShaderProgram(GLuint sprogram);

	
public:
	Utils();
	~Utils();
	static GLuint loadTexture(const char* texImagePath);
	static GLuint createShaderProgram(const char* vp, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
	static GLuint createShaderProgram(const char* vp, const char* fCS, const char* tES, const char* gp, const char* fp);


};