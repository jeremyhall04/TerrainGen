#include <iostream>
#include "engine/window/window.h"
#include "engine/utils/camera.h"
#include "engine/graphics/shader/shader.h"
#include "engine/utils/image/image.h"
#include "engine/graphics/2d/quad2d.h"

#include "engine/graphics/3d/terraingenerator.h"
//#include "engine/graphics/3d/terrainchunk.h

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

const int chunkSize = 240;
int LOD = 0; // 0, 1, 2, 4, 6, 8, 12

int main()
{
	Window window;
	g_Camera3d = new PerspectiveCamera(glm::vec3(chunkSize/2.0f, 700, chunkSize / 2.0f), glm::vec3(0, -1, 0), 45.0f);
	g_Camera2d = new OrthographicCamera();
	Shader* shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");

	/* TERRAIN */
	TerrainGenerator* terrainGen = new TerrainGenerator(chunkSize);
	//TerrainChunk** chunks = new TerrainChunk * [chunksVisible];
	//chunks[0] = new TerrainChunk(chunkSize, pos, seed, 0);
	//chunks[1] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 0.0f), seed, 1);
	//chunks[2] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, 1.0f), seed, 1);
	//chunks[3] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 0.0f), seed, 1);
	//chunks[4] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, -1.0f), seed, 1);
	//chunks[5] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 1.0f), seed, 2);
	//chunks[6] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 1.0f), seed, 2);
	//chunks[7] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, -1.0f), seed, 2);
	//chunks[8] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, -1.0f), seed, 2);
	//for (int i = 0; i < 9; i++)
	//	chunkMap.insert({ hash_vector(chunks[i]->getPosition()), chunks[i] });

	glm::vec3 light_color(255/255.0f, 254/255.0f, 230/255.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glm::vec3 light_pos;
	while (window.isRunning())
	{
		window.clear();
		window.processInput();

		terrainGen->update();

		shader->enable();  
		light_pos = g_Camera3d->getPosition();
		shader->setUniform3f("light_pos", glm::vec3(light_pos.x, 200.0f, light_pos.z));
		shader->setUniform3f("light_color", light_color);
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