#include "terrainchunk.h"
#include "../../noise/perlin.h"
#include "../../utils/image/image.h"
#include <random>

TerrainChunk::TerrainChunk() {}

TerrainChunk::TerrainChunk(int width, glm::vec3 pos, int amplification, int lod = 0)
	: pos(pos), amplification(amplification)
{
	this->width = width;
	height_data = new float[(width + 1) * (width + 1)];
	std::random_device rd;
	seed = rd();
	//seed = 1180901704;
	create_noise_map();

	shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");
	texture = new Texture("res/images/noise_map.png");

	mesh = new Mesh();
	triangleIndex = 0;
	lod = lod < 6 ? lod : 6;
	meshSimplificationIncrement = lod == 0 ? 1 : lod * 2;
	verticesPerLine = width / meshSimplificationIncrement;

	init();

	shader->enable();
	shader->setUniform1i("tex", 0);
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

	if (shader != NULL)
		delete shader;
	shader = NULL;
	if (texture != NULL)
		delete texture;

	if (height_data != NULL)
		delete height_data;
	height_data = NULL;
}

void TerrainChunk::init()
{
	mesh->init(pos, verticesPerLine, verticesPerLine);
	printf("\nVertices Loaded = %d\n", mesh->nVertices);

	int vIndex = 0;
	float ymin = 0, ymax = 0;

	// assigning heightmap
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
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->nVertices * sizeof(VertexData), mesh->vd, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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

void TerrainChunk::create_noise_map()
{
	noise::Perlin* p = new noise::Perlin(seed);
	Image* noise_map = new Image();
	noise_map->generate(width, width);

	// prepare noise texture
	float frequency = 4;
	float fx = noise_map->getWidth() / frequency;
	float fy = noise_map->getHeight() / frequency;
	float min = 0, max = 0;

	// get pixel color from perlin noise
	int index = 0;
	for (int y = 0; y <= width; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			float val = p->accumulatedNoise2D(x / fx, y / fy, 5, 2.0f, 0.5f);
			height_data[index] = val;
			index++;
			max = val > max ? val : max;
			min = val < min ? val : min;

			if (x < width && y < width)
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

void TerrainChunk::addTriangle(int a, int b, int c) {
	mesh->indices[triangleIndex] = a;
	mesh->indices[triangleIndex + 1] = b;
	mesh->indices[triangleIndex + 2] = c;
	triangleIndex += 3;
}

// function to smooth out slopes
float TerrainChunk::x_pow(float x)
{
	float y = std::pow(x, 5);
	return y;
}

void TerrainChunk::update()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->nVertices * sizeof(float), mesh->vd);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void TerrainChunk::render()
{
	texture->bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, triangleIndex, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, 0);
}
