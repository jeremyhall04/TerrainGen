#include <iostream>
//#include "engine/utils/timer.h"
#include "engine/window/window.h"
#include "engine/utils/camera.h"
#include "engine/graphics/shader/shader.h"
#include "engine/utils/image/image.h"
#include "engine/graphics/2d/quad2d.h"

#include "engine/graphics/3d/terraingenerator.h"
#include "engine/graphics/3d/terrainchunk.h"

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

const int mapChunkSize = 240;
int LOD = 0; // 0, 1, 2, 4, 6, 8, 12

int main()
{
	Window window;
	g_Camera3d = new PerspectiveCamera(glm::vec3(mapChunkSize/2.0f, 50, mapChunkSize / 2.0f), glm::vec3(0, -1, 0), 45.0f);
	g_Camera2d = new OrthographicCamera();
	Shader* shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");

	/* TERRAIN */
	//TerrainGenerator* terrain = new TerrainGenerator(mapChunkSize, glm::vec3(0.0f), heightmap, 60, LOD);
	TerrainChunk* terrain = new TerrainChunk(mapChunkSize, glm::vec3(0.0f), 100, LOD);

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