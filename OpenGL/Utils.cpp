#include "Utils.h"

Utils::Utils()
{
}

GLuint Utils::prepareShader(int shaderType, const char* shaderPath)
{
	GLint shaderCompiled;

	GLuint shader = glCreateShader(shaderType);
	GLuint program = glCreateProgram();

	string shaderStr = readShaderSource(shaderPath);
	const char* shaderSource = shaderStr.c_str();


	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	checkOpenGLError();
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != 1)
	{
		if (shaderType == GL_VERTEX_SHADER) cout << "GL_VERTEX_SHADER";
		if (shaderType == GL_TESS_CONTROL_SHADER) cout << "GL_TESS_CONTROL_SHADER";
		if (shaderType == GL_TESS_EVALUATION_SHADER) cout << "GL_TESS_EVALUATION_SHADER";
		if (shaderType == GL_GEOMETRY_SHADER) cout << "GL_GEOMETRY_SHADER";
		if (shaderType == GL_FRAGMENT_SHADER) cout << "GL_FRAGMENT_SHADER";
		cout << "shader compile error." << endl;
		printShaderLog(shader);
	}

	return shader;
}

bool Utils::checkOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

string Utils::readShaderSource(const char* filepath)
{
	string content;
	ifstream fileStream(filepath, ios::in);
	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

int Utils::finalizeShaderProgram(GLuint sprogram)
{
	GLint linked;
	glLinkProgram(sprogram);
	checkOpenGLError();
	glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		cout << "linking failed" << endl;
		printProgramLog(sprogram);
	}
	return sprogram;
}

void Utils::printShaderLog(GLuint shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void Utils::printProgramLog(int prog)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

GLuint Utils::createShaderProgram(const char* vp, const char* fp)
{
	return createShaderProgram(vp, nullptr, nullptr, nullptr, fp);
}

GLuint Utils::createShaderProgram(const char* vp, const char* gp, const char* fp)
{
	return createShaderProgram(vp, nullptr, nullptr, gp, fp);
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp)
{
	return createShaderProgram(vp, tCS, tES, nullptr, fp);
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp)
{
	GLuint vShader, tcShader, teShader, gShader, fShader;

	vShader = prepareShader(GL_VERTEX_SHADER, vp);
	if (tCS != nullptr)
		tcShader = prepareShader(GL_TESS_CONTROL_SHADER, tCS);
	if (tES != nullptr)
		teShader = prepareShader(GL_TESS_EVALUATION_SHADER, tES);
	if (gp != nullptr)
		gShader = prepareShader(GL_GEOMETRY_SHADER, gp);
	fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	if (tCS != nullptr)
		glAttachShader(program, tcShader);
	if (tES != nullptr)
		glAttachShader(program, teShader);
	if (gp != nullptr)
		glAttachShader(program, gShader);
	glAttachShader(program, fShader);
	
	return finalizeShaderProgram(program);
}


