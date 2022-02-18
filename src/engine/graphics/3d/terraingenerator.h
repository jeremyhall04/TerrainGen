#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "../../noise/perlin.h"
#include "mesh.h"
#include <stdlib.h>

class TerrainGenerator
{
private:
	// Generator properties
	glm::mat4 view;
	glm::vec3 pos;
	int chunkSize;
	int chunksVisible;

	Texture* texture;
	Shader* shader;

	// mesh properties
	Mesh* mesh;
	int width, resolution, amplification = 1;
	int triangleIndex = 0;
	int meshSimplificationIncrement, verticesPerLine;

public:
	TerrainGenerator();
	TerrainGenerator(int width, glm::vec3 pos, float* height_data, int amplification, int lod);
	~TerrainGenerator();

private:
	void init(glm::vec3 pos, float* height_data);
	void addTriangle(int a, int b, int c) {
		mesh->indices[triangleIndex] = a;
		mesh->indices[triangleIndex + 1] = b;
		mesh->indices[triangleIndex + 2] = c;
		triangleIndex += 3;
	}

public:
	void update();
	void render();
};

float x_pow(float x);

#endif // !TERRAIN_GENERATOR_H
