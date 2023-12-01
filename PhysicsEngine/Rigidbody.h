#pragma once

#include <glm\glm.hpp>

#include "CommonValues.h"

class Rigidbody
{
public:
	enum Shape
	{
		Cube, Sphere
	};
	Shape shapeType;
	float width;
	float height;
	float radius;

	Rigidbody();
	Rigidbody(glm::vec3 center, float rotZ, float mass, float elasticity, bool isStatic, float width, float height, float radius, Shape shapeType);

	void Update(GLfloat deltaTime);

	glm::vec3 center;
	glm::vec3 velocity;
	glm::vec3 angularVelocity;

	float rotX;
	float rotY;
	float rotZ;

	float mass;
	float inverseMass;
	float Inertia();
	float inverseInertia;

	float staticFrictionConstant;
	float dynamicFrictionConstant;
	float elasticity;
	bool isStatic;

	~Rigidbody();

private:
	void SetVelocity(GLfloat deltaTime);
	void SetAngularVelocity(GLfloat deltaTime);
	void AddForce(glm::vec3 forceVector, float deltaTime);
	void AddGravityForce(GLfloat deltaTime);
};

