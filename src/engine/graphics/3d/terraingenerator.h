#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "terrainchunk.h"
#include <map>

class TerrainGenerator
{
private:
	// Generator properties
	glm::mat4 view;
	glm::vec2 viewerPos;
	int chunkSize;
	int chunksVisible;
	float viewDistance;
	//std::map<glm::vec2, TerrainChunk*>* chunkMap;
	unsigned int seed;

	Shader* shader;
	TerrainChunk** chunks;

public:
	TerrainGenerator();
	TerrainGenerator(glm::vec3 pos, glm::mat4 view, int chunkSize);
	~TerrainGenerator();

private:
	void updateVisibleChunks();

public:
	void update();
	void render();
};

#endif // !TERRAIN_GENERATOR_H
