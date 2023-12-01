#pragma once

#include "Rigidbody.h"
#include <vector>

class PhysicsOperation
{
public:
	PhysicsOperation();

	static void LinearCollisionResponse(Rigidbody &rigidBody1, Rigidbody &rigidBody2, glm::vec3 normal);
	static void RotationalCollisionResponse(Rigidbody &rigidBody1, Rigidbody &rigidBody2, glm::vec3 normal, std::vector<glm::vec3> contactList);

	~PhysicsOperation();
};

