#include "terrainchunk.h"
#include "../../noise/perlin.h"
#include "../../utils/image/image.h"
#include <random>
#include <thread>

TerrainChunk::TerrainChunk() {}

TerrainChunk::TerrainChunk(int width, glm::vec3 chunkPos, unsigned int seed, int lod = 0)
{
	this->size = width;
	chunkPos *= width;
	this->pos = chunkPos;
	height_data = new float[(width + 1) * (width + 1)];
	amplification = 120;
	create_noise_map();

	texture = new Texture("res/images/noise_map.png");

	mesh = new Mesh();
	triangleIndex = 0;
	lod = lod < 6 ? lod : 6;
	meshSimplificationIncrement = lod == 0 ? 1 : lod * 2;
	verticesPerLine = width / meshSimplificationIncrement;
	is_visible = false;

	init();
}

TerrainChunk::~TerrainChunk()
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

	/*if (shader != NULL)
		delete shader;
	shader = NULL;*/
	if (texture != NULL)
		delete texture;

	if (height_data != NULL)
		delete height_data;
	height_data = NULL;
}

void TerrainChunk::init()
{
	mesh->init(pos, verticesPerLine, verticesPerLine);

	create_mesh();

	// setting data into buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->nVertices * sizeof(VertexData), mesh->vd, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndex * sizeof(GLuint), mesh->indices, GL_DYNAMIC_DRAW);
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

void TerrainChunk::create_mesh()
{
	int vIndex = 0;
	float ymin = 0, ymax = 0;

	// assigning heightmap
	for (int z = 0; z <= size; z += meshSimplificationIncrement)
	{
		for (int x = 0; x <= size; x += meshSimplificationIncrement)
		{
			float y = x_pow(height_data[z * (size + 1) + x]) * amplification;
			ymax = y > ymax ? y : ymax;
			ymin = y < ymin ? y : ymin;
			mesh->vd[vIndex].pos = glm::vec3(mesh->pos.x + x, mesh->pos.y + y, mesh->pos.z + z);
			mesh->vd[vIndex].uv = glm::vec2(x / (float)size, z / (float)size);
			if (x < size && z < size)
			{
				add_triangle(vIndex, vIndex + (verticesPerLine + 1), vIndex + (verticesPerLine + 1) + 1);
				add_triangle(vIndex, vIndex + (verticesPerLine + 1) + 1, vIndex + 1);
			}
			vIndex++;
		}
	}
	//printf("\nHeight max = %f | min = %f\n", ymax, ymin);

	// calculating normals
	vIndex = 0;
	glm::vec3 U(0.0f), V(0.0f), p1(0.0f), p2(0.0f), p3(0.0f), p4(0.0f);
	for (int z = 0; z <= size; z += meshSimplificationIncrement)
	{
		for (int x = 0; x <= size; x += meshSimplificationIncrement)
		{
			if (x < size && z < size)
			{
				p1 = mesh->vd[vIndex].pos;
				p2 = mesh->vd[vIndex + (verticesPerLine + 1)].pos;
				p3 = mesh->vd[vIndex + 1 + (verticesPerLine + 1)].pos;
				U = p2 - p1;
				V = p3 - p1;
				mesh->vd[vIndex].normal = glm::cross(U, V);
			}
			else if (z >= size)
				mesh->vd[vIndex].normal = mesh->vd[vIndex - (verticesPerLine + 1)].normal;
			else
				mesh->vd[vIndex].normal = mesh->vd[vIndex - 1].normal;
			vIndex++;
		}
	}

	//printf("\nLOD = %d\nVertices Loaded = %d", meshSimplificationIncrement, mesh->nVertices);
}

void TerrainChunk::create_noise_map()
{
	noise::Perlin* p = new noise::Perlin(seed);
	Image* noise_map = new Image();
	noise_map->generate(size, size);

	// prepare noise texture
	float frequency = 4;
	float fx = noise_map->getWidth() / frequency;
	float fy = noise_map->getHeight() / frequency;
	float min = 0, max = 0;

	// get pixel color from perlin noise
	int index = 0;
	for (int y = 0; y <= size; y++)
	{
		for (int x = 0; x <= size; x++)
		{
			float val = p->accumulatedNoise2D((x + pos.x) / fx, (y + pos.z) / fy, 5, 2.0f, 0.5f);
			height_data[index] = val;
			index++;
			max = val > max ? val : max;
			min = val < min ? val : min;

			if (x < size && y < size)
			{
				if (val < 0.45f)
					noise_map->setPixel(x, y, 0.2f, 0.4f, 1.0f);	// water
				else if (val < 0.48f)
					noise_map->setPixel(x, y, 0.87f, 0.86f, 0.69f);	// beach
				else if (val < 0.55f)
					noise_map->setPixel(x, y, 0.2f, 0.7f, 0.2f);	// grass light
				else if (val < 0.60f)
					noise_map->setPixel(x, y, 0.1f, 0.5f, 0.1f);	// grass dark
				else if (val < 0.63f)
					noise_map->setPixel(x, y, 0.3f, 0.2f, 0.15f);	// earth light
				else if (val < 0.70f)
					noise_map->setPixel(x, y, 0.2f, 0.1f, 0.1f);	// earth dark
				else if (val < 0.1)
					noise_map->setPixel(x, y, 1.0f, 0.98f, 0.98f);	// snow
			}
		}
	}

	//output noise map
	//printf("\nNoise_Map: \nSeed: %d\nfMin = %u | Max = %f\n\n", r, min, max);
	noise_map->write("res/images/noise_map.png");
	delete p;
	delete noise_map;
}

void TerrainChunk::add_triangle(int a, int b, int c) {
	mesh->indices[triangleIndex] = a;
	mesh->indices[triangleIndex + 1] = b;
	mesh->indices[triangleIndex + 2] = c;
	triangleIndex += 3;
}

float TerrainChunk::x_pow(float x)
{
	// function to smooth out slopes
	float y = std::pow(x, 5);
	return y;
}

void TerrainChunk::update(glm::vec3 pos, int lod)
{
	mesh->clear();
	meshSimplificationIncrement = lod;
	verticesPerLine = size / meshSimplificationIncrement;
	triangleIndex = 0;
	
	mesh->init(pos, verticesPerLine, verticesPerLine);
	create_mesh();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->nVertices * sizeof(VertexData), mesh->vd, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndex * sizeof(GLuint), mesh->indices, GL_DYNAMIC_DRAW);
}

void TerrainChunk::render()
{
	texture->bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, triangleIndex, GL_UNSIGNED_INT, 0);
}

void RequestMapData(void* callback)
{

}
