#ifndef MESH_H
#define MESH_H

#include "../../glHeader.h"
#include "../shader/shader.h"
#include "../texture/texture.h"
#include "../buffers/VAO.h"
#include "../buffers/VBO.h"
#include <vector>

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	//glm::vec4 color;
};

struct Mesh
{
	int width = 0, height = 0;
	VertexData* vd;
	GLuint* indices;
	GLuint nVertices, nIndices;
	glm::vec3 pos;

	void init(glm::vec3 pos, int width, int height)
	{
		this->width = width;
		this->height = height;
		this->pos = pos;
		nVertices = (width + 1) * (height + 1);
		nIndices = width * height * 6;
		vd = new VertexData[nVertices];
		indices = new GLuint[nIndices];
	}

	void clear()
	{
		delete vd;
		delete indices;
	}
};

#endif // !MESH_H

