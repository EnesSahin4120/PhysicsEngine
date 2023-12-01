#include "Line.h"

Line::Line()
{
}

Line::Line(glm::vec3 A, glm::vec3 B)
{
	this->A = A;
	this->B = B;
	v = glm::vec3(B.x - A.x, B.y - A.y, B.z - A.z);
}

glm::vec3 Line::Lerp(float t)
{
	t = glm::clamp(t, 0.0f, 1.0f);

	float xt = A.x + v.x * t;
	float yt = A.y + v.y * t;
	float zt = A.z + v.z * t;

	return glm::vec3(xt, yt, zt);
}


Line::~Line()
{
}
