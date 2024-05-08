#pragma once
#include "Utils.h"



class Sphere
{
private:
	int numVertices;
	int numIndices;
	vector<int> indices;
	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	void init(int);
	float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; };
public:
	Sphere();
	Sphere(int prec);
	~Sphere();
	int getNumVertices();
	int getNumIndices();
	vector<int> getIndices();
	vector<glm::vec3> getVertices();
	vector<glm::vec2> getTexCoords();
	vector<glm::vec3> getNormals();
	vector<glm::vec3> getTangents();
};

class Chapter6_1 :public ChapterBase {
private:
	float cameraX, cameraY, cameraZ;
	float sphLocX, sphLocY, sphLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];
	GLuint earthTexture;
	float rotAmt = 0.0f;

	// variable allocation for display
	GLuint mvLoc, projLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat;

	void setupVertices(void);
	Sphere mySphere = Sphere(48);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);

public:
	
	int run();
};
