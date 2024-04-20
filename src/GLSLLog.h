#pragma once
#include "OpenGL.h"

class  GLSLLog
{
public:
	GLSLLog();
	~GLSLLog();
	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();

};