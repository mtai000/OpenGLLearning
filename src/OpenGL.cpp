// OpenGL.cpp : Defines the entry point for the application.
//

#include "OpenGL.h"

#define numVAOs 1
using namespace std;

GLuint renderingProgram;
GLuint vao [numVAOs];

GLuint createShaderProgram(){
	const char *vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0,0.0,0.0,1.0)}";

	const char *fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ color = vec4(0.0,0.0,1.0,1.0)}";

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


void init(GLFWwindow* windows){
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs,vao);
	glBindVertexArray(vao[0]);
}


void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);
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

