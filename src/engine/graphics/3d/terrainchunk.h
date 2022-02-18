#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "mesh.h"

class Texture;
class Shader;

class TerrainChunk
{
private:
	// mesh properties
	int width, amplification = 1;
	int triangleIndex = 0;
	int meshSimplificationIncrement, verticesPerLine;

	Mesh* mesh;
	Texture* texture;
	//Shader* shader;

	GLuint vbo, vao, ebo;
	glm::vec3 pos;
	float* height_data;
	unsigned int seed;
	bool isVisible;

public:
	TerrainChunk();
	TerrainChunk(int width, glm::vec3 pos, unsigned int seed, int lod);
	~TerrainChunk();

private:
	void init();
	void create_noise_map();
	void create_mesh();
	void add_triangle(int a, int b, int c);
	float x_pow(float x);
	float getNearestChunkEdge(glm::vec2 viewerPosition);

public:
	void update(glm::vec3 pos, int lod);
	void render();
	void updatePos(glm::vec3 pos) { this->pos = pos; };
	void updateLOD(int lod) { meshSimplificationIncrement = lod; };
};

#endif