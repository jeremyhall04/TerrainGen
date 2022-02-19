#include <iostream>
#include "engine/window/window.h"
#include "engine/utils/camera.h"
#include "engine/graphics/shader/shader.h"
#include "engine/utils/image/image.h"
#include "engine/graphics/2d/quad2d.h"

#include "engine/graphics/3d/terraingenerator.h"
//#include "engine/graphics/3d/terrainchunk.h"
#include <random>
#include <map>

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

const int mapChunkSize = 240;
int LOD = 0; // 0, 1, 2, 4, 6, 8, 12

bool operator<(const glm::vec2& lhs, const glm::vec2& rhs)
{
	return lhs.x < rhs.x || lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y /*&& lhs.z < rhs.z*/);
}

char hash_vector(glm::vec2 pos);

int main()
{
	Window window;
	g_Camera3d = new PerspectiveCamera(glm::vec3(mapChunkSize/2.0f, 50, mapChunkSize / 2.0f), glm::vec3(0, -1, 0), 45.0f);
	g_Camera2d = new OrthographicCamera();
	Shader* shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");

	/* TERRAIN */
	TerrainGenerator* terrainGen = new TerrainGenerator(glm::vec3(0.0f), g_Camera3d->getViewMatrix(), mapChunkSize);

	std::random_device rd;
	unsigned int seed = rd();
	seed = 1180901704;

	std::map<char, TerrainChunk*> chunkMap;
	TerrainChunk chunk(mapChunkSize, glm::vec3(0.0f), seed, 0);
	glm::vec2 pos(-2.0f, 3.0f);
	char p = hash_vector(pos);
	chunkMap.insert({ p, &chunk });

	glm::vec3 light_color(255/255.0f, 254/255.0f, 230/255.0f);
	glm::vec3 light_pos(mapChunkSize/2.0f, 200.0f, mapChunkSize/2.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	unsigned int t = 0;
	bool done = false;

	while (window.isRunning())
	{
		window.clear();
		window.processInput();

		//terrainGen->update();

		shader->enable();  
		shader->setUniform3f("light_color", light_color);
		shader->setUniform3f("light_pos", light_pos);
		shader->setUniformMat4("projection", g_Camera3d->getProjectionMatrix());
		shader->setUniformMat4("view", g_Camera3d->getViewMatrix());

		terrainGen->render();

		window.update();
	}

	delete terrainGen;
	delete g_Camera3d;
	delete g_Camera2d;
	delete shader;

	return 0;
}

char hash_vector(glm::vec2 pos)
{
	char p = (char)0;
	if ((pos.x >= -3 && pos.x <= 3) && (pos.y >= -3 && pos.y <= 3))
	{
		int x = pos.x;
		int y = pos.y;
		bool nx = false, ny = false;
		nx = x < 0;
		ny = y < 0;
		char xx = glm::abs(x);
		char yy = glm::abs(y) << 2;
		char p = xx | yy;
		if (nx) p = p | (char)32;
		if (ny) p = p | (char)64;
	}
	else
		printf("\nError in hash_vector - position vector was out of range (-3,3)");
	return p;
}
