#include "terraingenerator.h"
#include <random>

TerrainGenerator::TerrainGenerator() {}

TerrainGenerator::TerrainGenerator(int width, glm::vec3 pos, float* height_data, int amplification, int lod = 0)
	: amplification(amplification)
{
	mesh = new Mesh();
	shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");
	texture = new Texture("res/images/noise_map.png");
	triangleIndex = 0;
	this->width = width;
	lod = lod < 6 ? lod : 6;
	meshSimplificationIncrement = lod == 0 ? 1 : lod * 2;
	verticesPerLine = width / meshSimplificationIncrement;

	init(pos, height_data);

	shader->enable();
	shader->setUniform1i("tex", 0);
}

TerrainGenerator::~TerrainGenerator()
{
	// MESH
	if (mesh != NULL)
	{
		if (mesh->vd != NULL)
			delete mesh->vd;
		mesh->vd = NULL;
		if (mesh->indices != NULL)
			delete[] mesh->indices;
		mesh->indices = NULL;
		delete mesh;
	}
	mesh = NULL;
}

void TerrainGenerator::init(glm::vec3 pos, float* height_data)
{
	mesh->init(pos, verticesPerLine, verticesPerLine);
	printf("\nVertices Loaded = %d\n", mesh->nVertices);

	int vIndex = 0;
	float ymin = 0, ymax = 0;

	// assigning heightmap
	//for (int z = 0; z <= width; z += meshSimplificationIncrement)
	//{
	//	for (int x = 0; x <= width; x += meshSimplificationIncrement)
	//	{
	//		float y = x_pow(height_data[vIndex]) * amplification;
	//		ymax = y > ymax ? y : ymax;
	//		ymin = y < ymin ? y : ymin;
	//		mesh->vd[vIndex].pos = glm::vec3(mesh->pos.x + x, mesh->pos.y + y, mesh->pos.z + z);
	//		//mesh->vd[vIndex].normal = glm::vec3(0.0f);
	//		mesh->vd[vIndex].color = glm::vec4(1.0f);
	//		mesh->vd[vIndex].uv = glm::vec2(x / (float)width, z / (float)width);
	//		if (x < width && z < width)
	//		{
	//			addTriangle(vIndex, vIndex + (verticesPerLine + 1), vIndex + (verticesPerLine + 1) + 1);
	//			addTriangle(vIndex, vIndex + (verticesPerLine + 1) + 1, vIndex + 1);
	//		}
	//		vIndex++;
	//	}
	//}
	for (int z = 0; z <= width; z += meshSimplificationIncrement)
	{
		for (int x = 0; x <= width; x += meshSimplificationIncrement)
		{
			float y = x_pow(height_data[z * (width + 1) + x]) * amplification;
			ymax = y > ymax ? y : ymax;
			ymin = y < ymin ? y : ymin;
			mesh->vd[vIndex].pos = glm::vec3(mesh->pos.x + x, mesh->pos.y + y, mesh->pos.z + z);
			//mesh->vd[vIndex].normal = glm::vec3(0.0f);
			//mesh->vd[vIndex].color = glm::vec4(1.0f);
			mesh->vd[vIndex].uv = glm::vec2(x / (float)width, z / (float)width);
			if (x < width && z < width)
			{
				addTriangle(vIndex, vIndex + (verticesPerLine + 1), vIndex + (verticesPerLine + 1) + 1);
				addTriangle(vIndex, vIndex + (verticesPerLine + 1) + 1, vIndex + 1);
			}
			vIndex++;
		}
	}
	printf("\nHeight max = %f | min = %f", ymax, ymin);

	// calculating normals
	vIndex = 0;
	glm::vec3 U(0.0f), V(0.0f), p1(0.0f), p2(0.0f), p3(0.0f), p4(0.0f);
	for (int z = 0; z <= width; z += meshSimplificationIncrement)
	{
		for (int x = 0; x <= width; x += meshSimplificationIncrement)
		{
			if (x < width && z < width)
			{
				p1 = mesh->vd[vIndex].pos;
				p2 = mesh->vd[vIndex + (verticesPerLine + 1)].pos;
				p3 = mesh->vd[vIndex + 1 + (verticesPerLine + 1)].pos;
				U = p2 - p1;
				V = p3 - p1;
				mesh->vd[vIndex].normal = glm::cross(U, V);
			}
			else if (z >= width)
				mesh->vd[vIndex].normal = mesh->vd[vIndex - (verticesPerLine + 1)].normal;
			else
				mesh->vd[vIndex].normal = mesh->vd[vIndex - 1].normal;
			vIndex++;
		}
	}

	// setting data into buffers
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->nVertices * sizeof(VertexData), mesh->vd, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndex * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);
	/*glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->nIndices * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);						// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3 * sizeof(float)));	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6 * sizeof(float)));	// uv
	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(8 * sizeof(float)));	// color
	//glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TerrainGenerator::update()
{
	int index = 0, vIndex = 0;
	GLuint currIndex = 0;
	float amplitude = 10;
	float ymin = 0, ymax = 0;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->nVertices * sizeof(float), mesh->vd);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
}

void TerrainGenerator::render()
{
	texture->bind();
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, triangleIndex, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, 0);
}

// function to smooth out slopes
float x_pow(float x)
{
	float y = std::pow(x, 3);
	return y;
}