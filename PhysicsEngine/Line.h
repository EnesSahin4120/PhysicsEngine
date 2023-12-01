#pragma once

#include <glm\glm.hpp>

class Line
{
public:
	Line();

	Line(glm::vec3 A, glm::vec3 B);
	glm::vec3 Lerp(float t);

	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 v;

	~Line();
};

