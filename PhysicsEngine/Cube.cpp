#include "Cube.h"



Cube::Cube()
{  }

void Cube::ClearMesh()
{
}

Cube::Cube(float width, float height, float depth, float angle, glm::vec3 pos, int rigidBodyIndex)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->angle = angle;
	this->pos = pos;
	this->rigidBodyIndex = rigidBodyIndex;

	halfLength[0] = width * 0.5f;
	halfLength[1] = height * 0.5f;
	halfLength[2] = depth * 0.5f;

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,    1.0f,  0.0f,0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,    1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f,0.0f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,0.0f,  0.0f,  -1.0f,
		0.5f, -0.5f,  0.5f,    1.0f,  0.0f,0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  0.5f,    1.0f,  1.0f,0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  0.5f,   1.0f,  1.0f,0.0f,  0.0f,  -1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,0.0f,  0.0f,  -1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,0.0f,  0.0f,  -1.0f,

		-0.5f,  0.5f,  0.5f,   1.0f,  0.0f,1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,   1.0f,  1.0f,1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,   1.0f,  0.0f,1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,    1.0f,  0.0f,-1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,    1.0f,  1.0f,-1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,    0.0f,  1.0f,-1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,   0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,    0.0f,  0.0f,-1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,   0.0f,  1.0f, 0.0f, 1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,    1.0f,  1.0f,0.0f, 1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,    1.0f,  0.0f,0.0f, 1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,    1.0f,  0.0f,0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  1.0f,0.0f, 1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,0.0f,  -1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,    1.0f,  1.0f,0.0f,  -1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,    1.0f,  0.0f,0.0f,  -1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,    1.0f,  0.0f,0.0f,  -1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,0.0f,  -1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  -1.0f,  0.0f
	};

	mesh = new Mesh();
	CreateMesh(vertices, 288);
}

void Cube::Update(glm::mat4 rotMatrix)
{
	axis[0] = glm::normalize(glm::vec3(rotMatrix[0]));
	axis[1] = glm::normalize(glm::vec3(rotMatrix[1]));
	axis[2] = glm::normalize(glm::vec3(rotMatrix[2]));

	rightDir = axis[0];
	upDir = axis[1];
	forwardDir = axis[2];

	upLeft = pos + (upDir * height * 0.5f) - (rightDir * width * 0.5f);
	upRight = pos + (upDir * height * 0.5f) + (rightDir * width * 0.5f);
	downRight = pos - (upDir * height * 0.5f) + (rightDir * width * 0.5f);
	downLeft = pos - (upDir * height * 0.5f) - (rightDir * width * 0.5f);

	vertices[0] = upLeft;
	vertices[1] = upRight;
	vertices[2] = downRight;
	vertices[3] = downLeft;
}

void Cube::CreateMesh(GLfloat *vertices, unsigned int numOfVertices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Cube::RenderMesh()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Cube::~Cube()
{
}
