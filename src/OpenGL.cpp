﻿// OpenGL.cpp : Defines the entry point for the application.
//

#include "OpenGL.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao [numVAOs];
GLSLLog *glog = new GLSLLog();

GLuint createShaderProgram_2_2(){
	const char *vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0,0.0,0.0,1.0);}";

	const char *fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ if(gl_FragCoord.x < 300) color = vec4(1.0,0.0,0.0,1.0); \
		   else color = vec4(0.0,1.0,0.0,1.0);}";

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader,1,&vshaderSource,NULL);
	glShaderSource(fshader,1,&fshaderSource,NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);

	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram,vshader);
	glAttachShader(vfProgram,fshader);
	glLinkProgram(vfProgram);

	return vfProgram;
}


GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	const char* vshaderSource =
		"#version 430    \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	const char* fshaderSource =
		"#version 430    \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vfprogram = glCreateProgram();

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);
	
	glog->checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled == 1) {
		cout << "vertex compilation success" << endl;
	}
	else {
		cout << "vertex compilation failed" << endl;
		glog->printShaderLog(vShader);
	}

	glCompileShader(fShader);
	glog->checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled == 1) {
		cout << "fragment compilation success" << endl;
	}
	else {
		cout << "fragment compilation failed" << endl;
		glog->printShaderLog(fShader);
	}

	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);

	glLinkProgram(vfprogram);
	glog->checkOpenGLError();
	glGetProgramiv(vfprogram, GL_LINK_STATUS, &linked);
	if (linked == 1) {
		cout << "linking succeeded" << endl;
	}
	else {
		cout << "linking failed" << endl;
		glog->printProgramLog(vfprogram);
	}

	return vfprogram;
}


void init(GLFWwindow* windows){
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs,vao);
	glBindVertexArray(vao[0]);
}


void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);
	glPointSize(30.0f);
	glDrawArrays(GL_POINTS,0,1);	
}

int main(void)
{
	if(!glfwInit()){exit(EXIT_FAILURE);}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	GLFWwindow* window=glfwCreateWindow(600,600,"Chapter2-program1",NULL,NULL);
	glfwMakeContextCurrent(window);
	if(glewInit()!=GLEW_OK){exit(EXIT_FAILURE);}
	glfwSwapInterval(1);

	init(window);

	while(!glfwWindowShouldClose(window)){
		display(window,glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

