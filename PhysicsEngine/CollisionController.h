#pragma once

#include <glm\glm.hpp>
#include "Sphere.h"
#include "Cube.h"
#include "Mathematics.h"

class CollisionController
{
public:
	CollisionController();

	static bool IsIntersectSpheres(Sphere sphere1, Sphere sphere2, float &depth, glm::vec3 &normal);
	static glm::vec3 ContactPointOfSpheres(Sphere sphere1, Sphere sphere2);

	static bool IsIntersectCubeSphere(Cube cube, Sphere sphere, glm::vec3 &contactPoint, float &depth, glm::vec3 &normal);

	static bool IsIntersectCubes(Cube cube1, Cube cube2);
	static void SetCubeCollisionValues(Cube cube1, Cube cube2, glm::vec3 &normal, float &depth);
	static void SetContactPointsOfCubes(Cube cube1, Cube cube2, glm::vec3 &contact1, glm::vec3 &contact2, int &contactCount);

	~CollisionController();
};

