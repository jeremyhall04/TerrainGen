#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "mesh.h"

class Texture;
class Shader;

class TerrainChunk
{
private:
	// mesh properties
	int width, resolution, amplification = 1;
	int triangleIndex = 0;
	int meshSimplificationIncrement, verticesPerLine;

	Mesh* mesh;
	Texture* texture;
	Shader* shader;

	GLuint vbo, vao, ebo;
	glm::vec3 pos;
	float* height_data;
	unsigned int seed;

public:
	TerrainChunk();
	TerrainChunk(int width, glm::vec3 pos, int amplification, int lod);
	~TerrainChunk();

private:
	void init();
	void create_noise_map();
	void addTriangle(int a, int b, int c);
	float x_pow(float x);

public:
	void update();
	void render();
};

#endif