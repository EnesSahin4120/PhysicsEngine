#include "CollisionController.h"



CollisionController::CollisionController()
{
}

bool CollisionController::IsIntersectSpheres(Sphere sphere1, Sphere sphere2, float &depth, glm::vec3 &normal)
{
	float distance = glm::distance(sphere1.pos, sphere2.pos);
	float radiusSum = sphere1.radius + sphere2.radius;

	if (distance >= radiusSum) {
		return false;
	}
	normal = glm::normalize(sphere2.pos - sphere1.pos);
	depth = radiusSum - distance;
	return true;
}

glm::vec3 CollisionController::ContactPointOfSpheres(Sphere sphere1, Sphere sphere2)
{
	glm::vec3 ab = sphere2.pos - sphere1.pos;
	glm::vec3 dir = glm::normalize(ab);
	return sphere1.pos + dir * sphere1.radius;
}

bool CollisionController::IsIntersectCubeSphere(Cube cube, Sphere sphere, glm::vec3 &contactPoint, float &depth, glm::vec3 &normal)
{
	glm::vec3 d = sphere.pos - cube.pos;
	contactPoint = cube.pos;

	for (int i = 0; i < 3; i++)
	{
		float dist = glm::dot(d, cube.axis[i]);

		if (dist > cube.halfLength[i]) dist = cube.halfLength[i];
		if (dist < -cube.halfLength[i]) dist = -cube.halfLength[i];

		contactPoint += dist * cube.axis[i];
	}

	glm::vec3 v = contactPoint - sphere.pos;
	normal = glm::normalize(v);

	if (glm::dot(v, v) <= sphere.radius * sphere.radius)
	{
		glm::vec3 dir = glm::normalize(contactPoint - sphere.pos);
		depth = glm::length(sphere.pos + dir * sphere.radius - contactPoint);
		return true;
	}
	else
	{
		depth = 0.0f;
		return false;
	}
}

bool CollisionController::IsIntersectCubes(Cube cube1, Cube cube2)
{
	float minA, maxA, minB, maxB;

	for (int i = 0; i < 4; i++)
	{
		glm::vec3 va = cube1.vertices[i];
		glm::vec3 vb = cube1.vertices[(i + 1) % 4];

		glm::vec3 edge = vb - va;
		glm::vec3 axis = glm::vec3(-edge.y, edge.x, 0);
		glm::normalize(axis);

		Mathematics::Calculate2DProjection(cube1.vertices, axis, minA, maxA);
		Mathematics::Calculate2DProjection(cube2.vertices, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return false;
	}
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 va = cube2.vertices[i];
		glm::vec3 vb = cube2.vertices[(i + 1) % 4];

		glm::vec3 edge = vb - va;
		glm::vec3 axis = glm::vec3(-edge.y, edge.x, 0);
		glm::normalize(axis);

		Mathematics::Calculate2DProjection(cube1.vertices, axis, minA, maxA);
		Mathematics::Calculate2DProjection(cube2.vertices, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return false;

	}
	return true;
}

void CollisionController::SetCubeCollisionValues(Cube cube1, Cube cube2, glm::vec3 &normal, float &depth) {
	depth = 100000.0f;

	for (int i = 0; i < 3; i++) {
		glm::vec3 axis = cube1.axis[i];

		float minProjection1, maxProjection1, minProjection2, maxProjection2;
		Mathematics::Calculate3DProjection(cube1, axis, minProjection1, maxProjection1);
		Mathematics::Calculate3DProjection(cube2, axis, minProjection2, maxProjection2);

		float axisDepth = glm::min(maxProjection2 - minProjection1, maxProjection1 - minProjection2);
		float projection1 = maxProjection2 - minProjection1;
		float projection2 = maxProjection1 - minProjection2;

		if (axisDepth < depth && projection1 != 0.0f && projection2 != 0.0f)
		{
			depth = axisDepth;
			normal = axis;
		}
	}
	for (int i = 0; i < 3; i++) {
		glm::vec3 axis = cube2.axis[i];

		float minProjection1, maxProjection1, minProjection2, maxProjection2;
		Mathematics::Calculate3DProjection(cube1, axis, minProjection1, maxProjection1);
		Mathematics::Calculate3DProjection(cube2, axis, minProjection2, maxProjection2);

		float axisDepth = glm::min(maxProjection2 - minProjection1, maxProjection1 - minProjection2);
		float projection1 = maxProjection2 - minProjection1;
		float projection2 = maxProjection1 - minProjection2;

		if (axisDepth < depth && projection1 != 0.0f && projection2 != 0.0f)
		{
			depth = axisDepth;
			normal = axis;
		}
	}
	for (int i = 0; i < 3; i++) {
		glm::vec3 axis = glm::cross(cube1.axis[0], cube2.axis[i]);

		float minProjection1, maxProjection1, minProjection2, maxProjection2;
		Mathematics::Calculate3DProjection(cube1, axis, minProjection1, maxProjection1);
		Mathematics::Calculate3DProjection(cube2, axis, minProjection2, maxProjection2);

		float axisDepth = glm::min(maxProjection2 - minProjection1, maxProjection1 - minProjection2);
		float projection1 = maxProjection2 - minProjection1;
		float projection2 = maxProjection1 - minProjection2;

		if (axisDepth < depth && projection1 != 0.0f && projection2 != 0.0f)
		{
			depth = axisDepth;
			normal = axis;
		}
	}
	for (int i = 0; i < 3; i++) {
		glm::vec3 axis = glm::cross(cube1.axis[1], cube2.axis[i]);

		float minProjection1, maxProjection1, minProjection2, maxProjection2;
		Mathematics::Calculate3DProjection(cube1, axis, minProjection1, maxProjection1);
		Mathematics::Calculate3DProjection(cube2, axis, minProjection2, maxProjection2);

		float axisDepth = glm::min(maxProjection2 - minProjection1, maxProjection1 - minProjection2);
		float projection1 = maxProjection2 - minProjection1;
		float projection2 = maxProjection1 - minProjection2;

		if (axisDepth < depth && projection1 != 0.0f && projection2 != 0.0f)
		{
			depth = axisDepth;
			normal = axis;
		}
	}
	for (int i = 0; i < 3; i++) {
		glm::vec3 axis = glm::cross(cube1.axis[2], cube2.axis[i]);

		float minProjection1, maxProjection1, minProjection2, maxProjection2;
		Mathematics::Calculate3DProjection(cube1, axis, minProjection1, maxProjection1);
		Mathematics::Calculate3DProjection(cube2, axis, minProjection2, maxProjection2);

		float axisDepth = glm::min(maxProjection2 - minProjection1, maxProjection1 - minProjection2);
		float projection1 = maxProjection2 - minProjection1;
		float projection2 = maxProjection1 - minProjection2;

		if (axisDepth < depth && projection1 != 0.0f && projection2 != 0.0f)
		{
			depth = axisDepth;
			normal = axis;
		}
	}
	if (glm::dot(cube2.pos - cube1.pos, normal) < 0.0f)
		normal = -normal;
}

void CollisionController::SetContactPointsOfCubes(Cube cube1, Cube cube2, glm::vec3 &contact1, glm::vec3 &contact2, int &contactCount)
{
	contact1 = glm::vec3(0, 0, 0);
	contact2 = glm::vec3(0, 0, 0);
	contactCount = 0;

	float minDistSq = 100000.0f;
	for (int i = 0; i < 4; i++) {
		glm::vec3 p = cube1.vertices[i];
		for (int j = 0; j < 4; j++) {
			glm::vec3 va = cube2.vertices[j];
			glm::vec3 vb = cube2.vertices[(j + 1) % 4];

			float distSq;
			glm::vec3 cp;
			Line line = Line(va, vb);
			Mathematics::LineSegmentToPoint(p, line, distSq, cp);
			bool isAlmostEqual = glm::abs(distSq - minDistSq) < 0.001f;
			if (isAlmostEqual)
			{
				isAlmostEqual = glm::distance(cp, contact1) < 0.1f;
				if (!isAlmostEqual)
				{
					contact2 = cp;
					contactCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				contactCount = 1;
				contact1 = cp;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		glm::vec3 p = cube2.vertices[i];
		for (int j = 0; j < 4; j++) {
			glm::vec3 va = cube1.vertices[j];
			glm::vec3 vb = cube1.vertices[(j + 1) % 4];

			float distSq;
			glm::vec3 cp;
			Line line = Line(va, vb);
			Mathematics::LineSegmentToPoint(p, line, distSq, cp);
			bool isAlmostEqual = glm::abs(distSq - minDistSq) < 0.001f;
			if (isAlmostEqual)
			{
				isAlmostEqual = glm::distance(cp, contact1) < 0.1f;
				if (!isAlmostEqual)
				{
					contact2 = cp;
					contactCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				contactCount = 1;
				contact1 = cp;
			}
		}
	}
}

CollisionController::~CollisionController()
{
}
