#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"
#include "CommonValues.h"

#pragma once
class Sphere
{
public:
	Sphere();
	Sphere(int sectorCount, int stackCount, float radius, glm::vec3 pos, float angle, int rigidBodyIndex);

	glm::vec3 pos;
	float radius;
	float angle;

	Mesh* mesh;

	int rigidBodyIndex;

	~Sphere();

private:
	GLfloat* vertices;
	unsigned int* indices;
	std::vector<GLfloat> verticesList;
	std::vector<unsigned int> indicesList;

	//Mesh Parameters
	float x, y, z, xy;
	float nx, ny, nz, lengthInv;
	float s, t;

	int k1, k2;

	float sectorStep;
	float stackStep;
	float sectorAngle, stackAngle;
};

