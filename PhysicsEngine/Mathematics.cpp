#include "Mathematics.h"



Mathematics::Mathematics()
{
}

void Mathematics::Calculate2DProjection(glm::vec3 vertices[], glm::vec3 axis, float & min, float & max)
{
	min = 100000.0f;
	max = -100000.0f;

	for (int i = 0; i< 4; i++)
	{
		glm::vec3 v = vertices[i];
		float proj = glm::dot(v, axis);
		if (proj < min) min = proj;
		if (proj > max) max = proj;
	}
}

void Mathematics::Calculate3DProjection(Cube cube, glm::vec3 axis, float &minProjection, float &maxProjection) {
	minProjection = maxProjection = glm::dot(cube.pos, axis);
	for (int i = 0; i < 3; ++i)
	{
		float projection = glm::dot(cube.axis[i], axis);
		float length = cube.halfLength[i] * glm::abs(projection);
		minProjection += -length;
		maxProjection += length;
	}
}

float Mathematics::DistanceSquared(glm::vec3 a, glm::vec3 b)
{
	float vx = a.x - b.x;
	float vy = a.y - b.y;
	return vx * vx + vy * vy;
}

void Mathematics::LineSegmentToPoint(glm::vec3 p, Line line, float & distanceSquared, glm::vec3 &cp)
{
	glm::vec3 ap = p - line.A;

	float proj = glm::dot(ap, line.v);
	float abLenSq = line.v.x * line.v.x + line.v.y * line.v.y;
	float d = proj / abLenSq;

	if (d <= 0.0f)
		cp = line.A;
	else if (d > 1.0f)
		cp = line.B;
	else
		cp = line.Lerp(d);


	distanceSquared = Mathematics::DistanceSquared(p, cp);
}

Mathematics::~Mathematics()
{
}
