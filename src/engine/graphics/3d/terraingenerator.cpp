#include "terraingenerator.h"
#include <random>
#include <iterator>

TerrainGenerator::TerrainGenerator() 
{
	shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");
}

TerrainGenerator::TerrainGenerator(glm::vec3 pos, glm::mat4 view, int chunkSize)
{
	this->chunkSize = chunkSize;
	chunksVisible = 9;
	viewDistance = chunkSize * 2;

	//chunkMap = new std::map<glm::vec2, TerrainChunk*>();

	std::random_device rd;
	seed = rd();
	//seed = 1180901704;

	chunks = new TerrainChunk * [chunksVisible];
	chunks[0] = new TerrainChunk(chunkSize, pos, seed, 0);
	chunks[1] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 0.0f), seed, 1);
	chunks[2] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, 1.0f), seed, 1);
	chunks[3] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 0.0f), seed, 1);
	chunks[4] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, -1.0f), seed, 1);
	chunks[5] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 1.0f), seed, 2);
	chunks[6] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 1.0f), seed, 2);
	chunks[7] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, -1.0f), seed, 2);
	chunks[8] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, -1.0f), seed, 2);
}

TerrainGenerator::~TerrainGenerator()
{
	for (int i = 0; i < chunksVisible; i++)
	{
		delete chunks[i];
		chunks[i] = NULL;
	}
	delete chunks;
}

//void TerrainGenerator::updateVisibleChunks()
//{
//	int currChunkCoordX = viewerPos.x / chunkSize;
//	int currChunkCoordY = viewerPos.y / chunkSize;
//
//	for (int yOffset = 0; yOffset < chunksVisible; yOffset++)
//	{
//		for (int xOffset = 0; xOffset < chunksVisible; xOffset++)
//		{
//			glm::vec2 viewedChunkCoord(currChunkCoordX + xOffset, currChunkCoordY + yOffset);
//
//			// dictionary of coordinates/chunks to keep track of ones we have created
//			std::map<glm::vec2, TerrainChunk*>::iterator it = chunkMap->find(viewedChunkCoord);
//			if (it != chunkMap->end())
//			{
//				// map does not contain this chunk coordinate
//
//			}
//			else
//			{
//				chunkMap->insert(std::pair<glm::vec2, TerrainChunk*>(viewedChunkCoord, 
//					new TerrainChunk(chunkSize, glm::vec3(viewedChunkCoord.x, 0.0f, viewedChunkCoord.y), seed, 2)));
//			}
//		}
//	}
//
//}

void TerrainGenerator::update()
{
	// check cam position & see if chunks need updating

	// only use updateVisibleChunks when moving from one chunk to another

	// use updateChunksLOD can be modified when the camera has moved enough in one direction

	// getting distance b/w cam 
	float distanceToChunkEdge = 0;
	bool visible = distanceToChunkEdge <= viewDistance;
}

void TerrainGenerator::render()
{
	for (int i = 0; i < chunksVisible; i++)
		chunks[i]->render();
}