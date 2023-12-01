#include "Sphere.h"



Sphere::Sphere()
{
}

Sphere::Sphere(int sectorCount, int stackCount, float radius, glm::vec3 pos, float angle, int rigidBodyIndex)
{
	this->radius = radius;
	this->pos = pos;
	this->rigidBodyIndex = rigidBodyIndex;
	this->angle = angle;

	lengthInv = 1.0f / radius;
	sectorStep = 2 * pi / sectorCount;
	stackStep = pi / stackCount;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = pi / 2.0f - i * stackStep;
		xy = radius * std::cos(stackAngle);
		z = radius * std::sin(stackAngle);

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			x = xy * std::cos(sectorAngle);
			y = xy * std::sin(sectorAngle);
			verticesList.push_back(x);
			verticesList.push_back(y);
			verticesList.push_back(z);

			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			verticesList.push_back(s);
			verticesList.push_back(t);

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			verticesList.push_back(-nx);
			verticesList.push_back(-ny);
			verticesList.push_back(-nz);
		}
	}


	for (int i = 0; i < stackCount; ++i)
	{
		k1 = (i * (sectorCount + 1));
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indicesList.push_back(k1);
				indicesList.push_back(k2);
				indicesList.push_back(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				indicesList.push_back(k1 + 1);
				indicesList.push_back(k2);
				indicesList.push_back(k2 + 1);
			}
		}
	}

	vertices = &verticesList[0];
	indices = &indicesList[0];

	mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, verticesList.size(), indicesList.size());
}


Sphere::~Sphere()
{
}
