#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "terrainchunk.h"
#include <map>
#include <list>

class TerrainGenerator
{
private:
	// Generator properties
	glm::mat4* view;
	glm::vec3 viewerPos;
	int chunkSize;
	int chunksVisible;
	float viewDistance;
	float maxViewDistance;
	std::map<char, TerrainChunk*>* chunkMap;
	std::map<char, TerrainChunk*>::iterator mapIterator;
	std::list<TerrainChunk*> chunksLastVisible;
	std::list<TerrainChunk*>::iterator listIterator;

	unsigned int seed;

	Shader* shader;
	TerrainChunk** chunks;

public:
	TerrainGenerator();
	TerrainGenerator(int chunkSize);
	~TerrainGenerator();
	void update();
	void render();

private:
	void updateVisibleChunks();
	void updateChunkVisibility(TerrainChunk* chunk);
	void clearList();
	float getNearestChunkEdge(TerrainChunk* chunk);
	glm::vec3 getPosFromViewMatrix();
	char hash_vector(glm::vec2 pos);
	char hash_vector(glm::vec3 pos);
};

#endif // !TERRAIN_GENERATOR_H
