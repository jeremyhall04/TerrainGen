#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "mesh.h"

class Texture;
class Shader;

class TerrainChunk
{
private:
	// mesh properties
	int size, amplification = 1;
	int triangleIndex = 0;
	int meshSimplificationIncrement, verticesPerLine;

	Mesh* mesh;
	Texture* texture;
	//Shader* shader;

	GLuint vbo, vao, ebo;
	glm::vec3 pos;
	float* height_data;
	unsigned int seed;
	bool is_visible;

public:
	TerrainChunk();
	TerrainChunk(int size, glm::vec3 chunkPos, unsigned int seed, int lod);
	~TerrainChunk();

private:
	void init();
	void create_noise_map();
	void create_mesh();
	void add_triangle(int a, int b, int c);
	float x_pow(float x);

protected:
	//void updateChunk(glm::vec3 viewerPosition);

public:
	void update(glm::vec3 pos, int lod);
	void render();
	void updatePos(glm::vec3 pos) { this->pos = pos; };
	void updateLOD(int lod) { meshSimplificationIncrement = lod; };
	glm::vec3 getPosition() { return pos / 240.0f; };
	int getSize() { return size; };
	void setVisibility(bool visible) { is_visible = visible; };
	bool isVisible() { return is_visible; };

	//float getNearestEdge(glm::vec3 playerPos);
};

void RequestMapData(void* callback);

#endif