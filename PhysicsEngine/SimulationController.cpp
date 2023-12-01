#include "SimulationController.h"

SimulationController::SimulationController()
{
}

void SimulationController::SphereCollisions(std::vector<Sphere> &spheres, std::vector<Rigidbody>& rigidBodies)
{
	for (int i = 0; i < spheres.size(); i++) {
		for (int j = i + 1; j < spheres.size(); j++) {
			std::vector<glm::vec3> contactPoints;
			glm::vec3 contactPoint;

			float depth;
			glm::vec3 normal;
			bool isIntersect = CollisionController::IsIntersectSpheres(spheres[i], spheres[j], depth, normal);
			if (isIntersect) {
				contactPoint = CollisionController::ContactPointOfSpheres(spheres[i], spheres[j]);
				contactPoints.push_back(contactPoint);

				if (rigidBodies[spheres[i].rigidBodyIndex].isStatic)
					rigidBodies[spheres[j].rigidBodyIndex].center += normal *depth;
				else if (rigidBodies[spheres[j].rigidBodyIndex].isStatic)
					rigidBodies[spheres[i].rigidBodyIndex].center -= normal *depth;
				else
				{
					rigidBodies[spheres[i].rigidBodyIndex].center -= normal * depth * 0.5f;
					rigidBodies[spheres[j].rigidBodyIndex].center += normal * depth * 0.5f;
				}
				PhysicsOperation::RotationalCollisionResponse(rigidBodies[spheres[i].rigidBodyIndex], rigidBodies[spheres[j].rigidBodyIndex], normal, contactPoints);
			}
		}
	}
}

void SimulationController::SphereCubeCollisions(std::vector<Sphere>& spheres, std::vector<Cube>& cubes, std::vector<Rigidbody>& rigidBodies)
{
	for (int i = 0; i < cubes.size(); i++) {
		for (int j = 0; j < spheres.size(); j++) {
			std::vector<glm::vec3> contactPoints;
			glm::vec3 contactPoint;

			float depth;
			glm::vec3 normal;
			bool isIntersect = CollisionController::IsIntersectCubeSphere(cubes[i], spheres[j], contactPoint, depth, normal);
			if (isIntersect) {
				contactPoints.push_back(contactPoint);

				if (rigidBodies[spheres[j].rigidBodyIndex].isStatic)
					rigidBodies[cubes[i].rigidBodyIndex].center += normal *depth;
				else if (rigidBodies[cubes[i].rigidBodyIndex].isStatic)
					rigidBodies[spheres[j].rigidBodyIndex].center -= normal *depth;
				else
				{
					rigidBodies[spheres[j].rigidBodyIndex].center -= normal * depth * 0.5f;
					rigidBodies[cubes[i].rigidBodyIndex].center += normal * depth * 0.5f;
				}
				PhysicsOperation::RotationalCollisionResponse(rigidBodies[spheres[j].rigidBodyIndex], rigidBodies[cubes[i].rigidBodyIndex], normal, contactPoints);
			}
		}
	}
}

void SimulationController::CubeCollisions(std::vector<Cube>& cubes, std::vector<Rigidbody>& rigidBodies)
{
	for (int i = 0; i < cubes.size(); i++) {
		for (int j = i + 1; j < cubes.size(); j++) {
			std::vector<glm::vec3> contactPoints;
			glm::vec3 contactPoint1;
			glm::vec3 contactPoint2;
			int contactCount;

			float depth;
			glm::vec3 normal;
			bool isIntersect = CollisionController::IsIntersectCubes(cubes[i], cubes[j]);
			if (isIntersect) {
				CollisionController::SetCubeCollisionValues(cubes[i], cubes[j], normal, depth);
				CollisionController::SetContactPointsOfCubes(cubes[i], cubes[j], contactPoint1, contactPoint2, contactCount);

				if (contactCount = 1)
					contactPoints.push_back(contactPoint1);
				else {
					contactPoints.push_back(contactPoint1);
					contactPoints.push_back(contactPoint2);
				}

				if (rigidBodies[cubes[i].rigidBodyIndex].isStatic)
					rigidBodies[cubes[j].rigidBodyIndex].center += normal *depth;
				else if (rigidBodies[cubes[j].rigidBodyIndex].isStatic)
					rigidBodies[cubes[i].rigidBodyIndex].center -= normal *depth;
				else
				{
					rigidBodies[cubes[i].rigidBodyIndex].center -= normal * depth * 0.5f;
					rigidBodies[cubes[j].rigidBodyIndex].center += normal * depth * 0.5f;
				}
				PhysicsOperation::RotationalCollisionResponse(rigidBodies[cubes[i].rigidBodyIndex], rigidBodies[cubes[j].rigidBodyIndex], normal, contactPoints);
			}
		}
	}
}

SimulationController::~SimulationController()
{
}
