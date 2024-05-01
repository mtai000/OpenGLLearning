// OpenGL.cpp : Defines the entry point for the application.
//

#include "OpenGL.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];
Utils* utils = new Utils();

float x = 0.0f;
float step = 0.01f;


/*

GLuint createShaderProgram_2_2() {
	const char* vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0,0.0,0.0,1.0);}";

	const char* fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ if(gl_FragCoord.x < 300) color = vec4(1.0,0.0,0.0,1.0); \
		   else color = vec4(0.0,1.0,0.0,1.0);}";

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, &vshaderSource, NULL);
	glShaderSource(fshader, 1, &fshaderSource, NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);

	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vshader);
	glAttachShader(vfProgram, fshader);
	glLinkProgram(vfProgram);

	return vfProgram;
}


GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	//const char *vshaderSource =
	//	"#version 430 \n"
	//	"void main(void) \n"
	//	"{ gl_Position = vec4(0.0,0.0,0.0,1.0);}";

	//const char *fshaderSource =
	//	"#version 430 \n"
	//	"out vec4 color; \n"
	//	"void main(void) \n"
	//	"{ if(gl_FragCoord.x < 300) color = vec4(1.0,0.0,0.0,1.0); \
	//	   else color = vec4(0.0,1.0,0.0,1.0);}";
	string verShaderStr = readShaderSource("vertShader.glsl");
	string fraghaderStr = readShaderSource("fragShader.glsl");

	const char* vshaderSource = verShaderStr.c_str();
	const char* fshaderSource = fraghaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vfprogram = glCreateProgram();

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);

	utils->checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled == 1) {
		cout << "vertex compilation success" << endl;
	}
	else {
		cout << "vertex compilation failed" << endl;
		utils->printShaderLog(vShader);
	}

	glCompileShader(fShader);
	utils->checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled == 1) {
		cout << "fragment compilation success" << endl;
	}
	else {
		cout << "fragment compilation failed" << endl;
		utils->printShaderLog(fShader);
	}

	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);

	glLinkProgram(vfprogram);
	utils->checkOpenGLError();
	glGetProgramiv(vfprogram, GL_LINK_STATUS, &linked);
	if (linked == 1) {
		cout << "linking succeeded" << endl;
	}
	else {
		cout << "linking failed" << endl;
		utils->printProgramLog(vfprogram);
	}

	return vfprogram;
}
*/

void init(GLFWwindow* windows)
{
	const char* vS = "vertShader.glsl";
	const char* fS = "fragShader.glsl";
	renderingProgram = Utils::createShaderProgram(vS, fS);
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}


void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	x += step;
	if (x > 0.75f || x < -0.75f) step = -step;
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");

	cout << currentTime << endl;
	glUseProgram(renderingProgram);
	glProgramUniform1f(renderingProgram, offsetLoc, x);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void)
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2-program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

