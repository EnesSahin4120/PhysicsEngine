#include "PhysicsOperation.h"



PhysicsOperation::PhysicsOperation()
{
}

void PhysicsOperation::LinearCollisionResponse(Rigidbody & rigidBody1, Rigidbody & rigidBody2, glm::vec3 normal)
{
	glm::vec3 relativeVelocity = rigidBody2.velocity - rigidBody1.velocity;
	if (glm::dot(relativeVelocity, normal) > 0.0f)
		return;

	float e = glm::min(rigidBody1.elasticity, rigidBody2.elasticity);
	float j = -(1.0f + e) * glm::dot(relativeVelocity, normal);
	j /= (rigidBody1.inverseMass + rigidBody2.inverseMass);

	rigidBody1.velocity -= j * rigidBody1.inverseMass * normal;
	rigidBody2.velocity += j * rigidBody2.inverseMass * normal;
}

void PhysicsOperation::RotationalCollisionResponse(Rigidbody & rigidBody1, Rigidbody & rigidBody2, glm::vec3 normal, std::vector<glm::vec3> contactList)
{
	float e = glm::min(rigidBody1.elasticity, rigidBody2.elasticity);
	float staticFriction = (rigidBody1.staticFrictionConstant + rigidBody2.staticFrictionConstant) * 0.5f;
	float dynamicFriction = (rigidBody1.dynamicFrictionConstant + rigidBody2.dynamicFrictionConstant) * 0.5f;
	float js[2];

	for (int i = 0; i < contactList.size(); i++) {
		glm::vec3 r1 = contactList[i] - rigidBody1.center;
		glm::vec3 r2 = contactList[i] - rigidBody2.center;

		glm::vec3 velocity1 = glm::vec3(-r1.y, r1.x, 0) * rigidBody1.angularVelocity.z;
		glm::vec3 velocity2 = glm::vec3(-r2.y, r2.x, 0) * rigidBody2.angularVelocity.z;

		glm::vec3 relativeVelocity = (rigidBody2.velocity + velocity2) -
			(rigidBody1.velocity + velocity1);

		if (glm::dot(relativeVelocity, normal) > 0.0f)
			continue;

		float denom = rigidBody1.inverseMass + rigidBody2.inverseMass +
			(glm::dot(glm::vec3(-r1.y, r1.x, 0), normal) *  glm::dot(glm::vec3(-r1.y, r1.x, 0), normal)) * rigidBody1.inverseInertia +
			(glm::dot(glm::vec3(-r2.y, r2.x, 0), normal) * glm::dot(glm::vec3(-r2.y, r2.x, 0), normal)) * rigidBody2.inverseInertia;

		float j = -(1.0f + e) * glm::dot(relativeVelocity, normal);
		j /= denom;
		j /= (float)contactList.size();
		js[i] = j;

		glm::vec3 impulse = j * normal;

		rigidBody1.velocity -= impulse * rigidBody1.inverseMass;
		rigidBody1.angularVelocity -= glm::cross(r1, impulse) * rigidBody1.inverseInertia;
		rigidBody2.velocity += impulse * rigidBody2.inverseMass;
		rigidBody2.angularVelocity += glm::cross(r2, impulse) * rigidBody2.inverseInertia;
	}

	for (int i = 0; i < contactList.size(); i++) {
		glm::vec3 r1 = contactList[i] - rigidBody1.center;
		glm::vec3 r2 = contactList[i] - rigidBody2.center;

		glm::vec3 velocity1 = glm::vec3(-r1.y, r1.x, 0) * rigidBody1.angularVelocity.z;
		glm::vec3 velocity2 = glm::vec3(-r2.y, r2.x, 0) * rigidBody2.angularVelocity.z;

		glm::vec3 relativeVelocity = (rigidBody2.velocity + velocity2) -
			(rigidBody1.velocity + velocity1);

		glm::vec3 tangent = relativeVelocity - glm::dot(relativeVelocity, normal) * normal;
		if (glm::distance(tangent, glm::vec3(0, 0, 0)) < 0.1f)
			continue;
		else
			tangent = glm::normalize(tangent);

		float denom = rigidBody1.inverseMass + rigidBody2.inverseMass +
			(glm::dot(glm::vec3(-r1.y, r1.x, 0), tangent) * glm::dot(glm::vec3(-r1.y, r1.x, 0), tangent)) * rigidBody1.inverseInertia +
			(glm::dot(glm::vec3(-r2.y, r2.x, 0), tangent) * glm::dot(glm::vec3(-r2.y, r2.x, 0), tangent)) * rigidBody2.inverseInertia;

		float jt = -glm::dot(relativeVelocity, tangent);
		jt /= denom;
		jt /= (float)contactList.size();

		glm::vec3 frictionImpulse;

		if (glm::abs(jt) <= js[i] * staticFriction)
			frictionImpulse = jt * tangent;
		else
			frictionImpulse = -js[i] * tangent * dynamicFriction;

		if (glm::abs(glm::length(frictionImpulse)) < 10.0f)
		{
			rigidBody1.velocity -= frictionImpulse * rigidBody1.inverseMass;
			rigidBody1.angularVelocity -= glm::cross(r1, frictionImpulse) * rigidBody1.inverseInertia;
			rigidBody2.velocity += frictionImpulse * rigidBody2.inverseMass;
			rigidBody2.angularVelocity += glm::cross(r2, frictionImpulse) * rigidBody2.inverseInertia;
		}
	}
}

PhysicsOperation::~PhysicsOperation()
{
}
