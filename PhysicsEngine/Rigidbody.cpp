#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
}

Rigidbody::Rigidbody(glm::vec3 center, float rotZ, float mass, float elasticity, bool isStatic, float width, float height, float radius, Shape shapeType)
{
	this->center = center;
	this->rotZ = rotZ;
	this->mass = mass;
	this->elasticity = elasticity;
	this->isStatic = isStatic;
	this->width = width;
	this->height = height;
	this->radius = radius;
	this->shapeType = shapeType;

	staticFrictionConstant = 0.2f;
	dynamicFrictionConstant = 0.2f;

	velocity = glm::vec3(0, 0, 0);
	angularVelocity = glm::vec3(0, 0, 0);

	rotX = 0.0f;
	rotY = 0.0f;

	if (!isStatic) {
		inverseMass = 1.0f / mass;
		inverseInertia = 1.0f / Inertia();
	}
	else {
		inverseMass = 0.0f;
		inverseInertia = 0.0f;
	}
}

float Rigidbody::Inertia()
{
	if(shapeType == Shape::Sphere)
	    return 0.5f * mass * radius * radius;
	else
		return (1.0f / 12.0f) * mass * (width * width + height * height);
}

void Rigidbody::AddGravityForce(GLfloat deltaTime)
{
	glm::vec3 distanceVector = center - glm::vec3(0.0f, -6371000.0f, 0.0f);
	float distanceMagnitude = glm::length(distanceVector);
	float gravityMagnitude = G * mass * 5.972e+24f / glm::pow(distanceMagnitude, 2);

	glm::vec3 gravityVector = gravityMagnitude * glm::normalize(distanceVector);
	AddForce(-gravityVector, deltaTime);
}

void Rigidbody::SetVelocity(GLfloat deltaTime)
{
	center += velocity * deltaTime;
}

void Rigidbody::SetAngularVelocity(GLfloat deltaTime)
{
	rotX += angularVelocity.x * deltaTime;
	rotY += angularVelocity.y * deltaTime;
	rotZ += angularVelocity.z * deltaTime;
}

void Rigidbody::Update(GLfloat deltaTime)
{
	if (!isStatic) {
		inverseMass = 1.0f / mass;
		inverseInertia = 1.0f / Inertia();

		AddGravityForce(deltaTime);
		SetVelocity(deltaTime);
		SetAngularVelocity(deltaTime);
	}
	else {
		inverseMass = 0.0f;
		inverseInertia = 0.0f;
	}
}

void Rigidbody::AddForce(glm::vec3 forceVector, float deltaTime)
{
	//Runge Kutta Integration
	float k1, k2, k3, k4;

	float force = glm::distance(forceVector, glm::vec3(0, 0, 0));
	float acceleration = force / mass;
	k1 = acceleration * deltaTime;

	force -= (glm::distance(velocity, glm::vec3(0, 0, 0)) + (k1 * 0.5f));
	acceleration = force / mass;
	k2 = acceleration * deltaTime;

	force -= (glm::distance(velocity, glm::vec3(0, 0, 0)) + (k2 * 0.5f));
	acceleration = force / mass;
	k3 = acceleration * deltaTime;

	force -= (glm::distance(velocity, glm::vec3(0, 0, 0)) + k3);
	acceleration = force / mass;
	k4 = acceleration * deltaTime;

	velocity += glm::normalize(forceVector) * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0f;
}

Rigidbody::~Rigidbody()
{
}
