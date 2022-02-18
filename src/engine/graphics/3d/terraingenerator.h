#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "terrainchunk.h"

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



public:
	TerrainGenerator();
	~TerrainGenerator();

private:

public:
	void update();
	void render();
};

float x_pow(float x);

#endif // !TERRAIN_GENERATOR_H
