#include <iostream>
//#include "engine/utils/timer.h"
#include "engine/window/window.h"
#include "engine/utils/camera.h"
#include "engine/graphics/shader/shader.h"
#include "engine/graphics/2d/quad2d.h"
#include "engine/graphics/3d/terraingenerator.h"
#include "engine/utils/image/image.h"

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

const int mapChunkSize = 240;
int LOD = 0; // 1, 2, 4, 6, 8, 12

void create_noise_map(const int imgW, float* heightmap);

class MeshData
{
public:
	glm::vec3* vertices;
	glm::vec2* uvs;
	GLuint* indices;
	GLuint nVertices, nIndices;
	int* triangles;
	int triangleIndex;
	int width = 0, height = 0;

	MeshData() {};

	MeshData(int meshWidth, int meshHeight)
	{
		width = meshWidth;
		height = meshHeight;
		vertices = new glm::vec3[(meshWidth + 1) * (meshHeight + 1)];
		uvs = new glm::vec2[(meshWidth + 1) * (meshHeight + 1)];
		triangles = new int[(meshWidth) * (meshHeight) * 6];
		triangleIndex = 0;
	}

	void addTriangle(int a, int b, int c)
	{
		triangles[triangleIndex] = a;
		triangles[triangleIndex + 1] = b;
		triangles[triangleIndex + 2] = c;
		triangleIndex += 3;
	}

	Mesh createMesh()
	{
		Mesh* mesh = new Mesh();
		mesh->init(glm::vec3(-width / 2.0f, 0.0f, -height / 2.0f), width, height);
	}
};

int main()
{
	Window window;
	g_Camera3d = new PerspectiveCamera(glm::vec3(mapChunkSize/2.0f, 50, mapChunkSize / 2.0f), glm::vec3(0, -1, 0), 45.0f);
	g_Camera2d = new OrthographicCamera();
	Shader* shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");

	/* NOISE IMAGE */
	float* heightmap = new float[(mapChunkSize + 1) * (mapChunkSize + 1)];
	create_noise_map(mapChunkSize, heightmap);
	//Quad2D* quad = new Quad2D(glm::vec3(0.0f), "res/shaders/quad.vert", "res/shaders/quad.frag", "res/images/noise_map.png");

	/* TERRAIN */
	TerrainGenerator* terrain = new TerrainGenerator(mapChunkSize, glm::vec3(0.0f), heightmap, 60, LOD);

	glm::vec3 light_color(255/255.0f, 254/255.0f, 230/255.0f);
	glm::vec3 light_pos(mapChunkSize/2.0f, 200.0f, mapChunkSize/2.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	while (window.isRunning())
	{
		window.clear();
		window.processInput();

		//terrain->update();

		shader->enable();  
		shader->setUniform3f("light_color", light_color);
		shader->setUniform3f("light_pos", light_pos);
		shader->setUniformMat4("projection", g_Camera3d->getProjectionMatrix());
		shader->setUniformMat4("view", g_Camera3d->getViewMatrix());

		terrain->render();

		window.update();
	}

	delete terrain;
	delete g_Camera3d;
	delete g_Camera2d;
	delete shader;

	return 0;
}

void create_noise_map(const int imgW, float* heightmap)
{
	std::random_device rd;
	unsigned int r = rd();
	//r = 1180901704;
	noise::Perlin* p = new noise::Perlin(r);
	Image* noise_map = new Image();
	noise_map->generate(imgW, imgW);

	// prepare noise texture
	float frequency = 4;
	float fx = noise_map->getWidth() / frequency;
	float fy = noise_map->getHeight() / frequency;
	float min = 0, max = 0;

	// get pixel color from perlin noise
	int index = 0;
	for (int y = 0; y <= imgW; y++)
	{
		for (int x = 0; x <= imgW; x++)
		{
			float val = p->accumulatedNoise2D(x / fx, y / fy, 5, 2.0f, 0.5f);
			heightmap[index] = val;
			index++;
			max = val > max ? val : max;
			min = val < min ? val : min;

			if (x < imgW && y < imgW)
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
	printf("\nNoise_Map: \nSeed: %d\nfMin = %u | Max = %f\n\n", r, min, max);
	noise_map->write("res/images/noise_map.png");

	delete p;
	delete noise_map;
}
