#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "../../noise/perlin.h"
#include "mesh.h"
#include <stdlib.h>

class TerrainGenerator
{
private:
	// mesh properties
	int width, resolution, amplification = 1;
	int triangleIndex = 0;
	int meshSimplificationIncrement, verticesPerLine;

	Mesh* mesh;
	Texture* texture;
	Shader* shader;

public:
	TerrainGenerator();
	TerrainGenerator(int width, glm::vec3 pos, float* height_data, int amplification, int lod);
	~TerrainGenerator();

	void init(glm::vec3 pos, float* height_data);
	void update();
	void render();

private:
	void addTriangle(int a, int b, int c) {
		mesh->indices[triangleIndex] = a;
		mesh->indices[triangleIndex + 1] = b;
		mesh->indices[triangleIndex + 2] = c;
		triangleIndex += 3;
	}
};

float x_pow(float x);

#endif // !TERRAIN_GENERATOR_H
