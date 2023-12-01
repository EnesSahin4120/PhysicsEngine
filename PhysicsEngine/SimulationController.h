#pragma once

#include"CollisionController.h"
#include "PhysicsOperation.h"

class SimulationController
{
public:
	SimulationController();

	static void SphereCollisions(std::vector<Sphere> &spheres, std::vector<Rigidbody> &rigidBodies);
	static void CubeCollisions(std::vector<Cube> &cubes, std::vector<Rigidbody> &rigidBodies);
	static void SphereCubeCollisions(std::vector<Sphere> &spheres, std::vector<Cube> &cubes, std::vector<Rigidbody> &rigidBodies);

	~SimulationController();
};

