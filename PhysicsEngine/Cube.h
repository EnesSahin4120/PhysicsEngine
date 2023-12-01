#pragma once

#include <glm\glm.hpp>

#include "Mesh.h"

class Cube
{
public:
	Cube();
	Cube(float width, float height, float depth, float angle, glm::vec3 pos, int rigidBodyIndex);
	void RenderMesh();

	float width;
	float height;
	float depth;
	glm::vec3 pos;
	float angle;

	glm::vec3 vertices[4];
	void Update(glm::mat4 rotMatrix);

	float halfLength[3];
	glm::vec3 axis[3];

	int rigidBodyIndex;

	~Cube();

private:
	Mesh* mesh;

	void CreateMesh(GLfloat *vertices, unsigned int numOfVertices);
	void ClearMesh();

	glm::vec3 upLeft;
	glm::vec3 upRight;
	glm::vec3 downRight;
	glm::vec3 downLeft;

	glm::vec3 upDir;
	glm::vec3 rightDir;
	glm::vec3 forwardDir;

    GLuint VAO, VBO;
	GLsizei indexCount;
};

