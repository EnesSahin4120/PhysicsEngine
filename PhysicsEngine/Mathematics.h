#pragma once

#include"Line.h"
#include "Cube.h"

class Mathematics
{
public:
	Mathematics();

	static void Calculate2DProjection(glm::vec3 vertices[], glm::vec3 axis, float &min, float &max);
	static void Calculate3DProjection(Cube cube, glm::vec3 axis, float &minProjection, float &maxProjection);
	static void LineSegmentToPoint(glm::vec3 p, Line line, float & distanceSquared, glm::vec3 &cp);
	static float DistanceSquared(glm::vec3 a, glm::vec3 b);

	~Mathematics();
};

