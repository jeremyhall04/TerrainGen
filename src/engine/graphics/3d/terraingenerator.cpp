#include "terraingenerator.h"
#include "../../utils/camera.h"
#include <random>
#include <iterator>

TerrainGenerator::TerrainGenerator() 
{
	shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");
}

TerrainGenerator::TerrainGenerator(int chunkSize)
{
	this->chunkSize = chunkSize;
	maxViewDistance = 300.0f;
	chunksVisible = (int)glm::round(maxViewDistance / chunkSize);

	chunkMap = new std::map<char, TerrainChunk*>();

	std::random_device rd;
	seed = rd();
	seed = 1180901704;

	//chunks = new TerrainChunk * [chunksVisible];
	//chunks[0] = new TerrainChunk(chunkSize, pos, seed, 0);
	//chunks[1] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 0.0f), seed, 1);
	//chunks[2] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, 1.0f), seed, 1);
	//chunks[3] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 0.0f), seed, 1);
	//chunks[4] = new TerrainChunk(chunkSize, pos + glm::vec3(0.0f, 0.0f, -1.0f), seed, 1);
	//chunks[5] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, 1.0f), seed, 2);
	//chunks[6] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, 1.0f), seed, 2);
	//chunks[7] = new TerrainChunk(chunkSize, pos + glm::vec3(-1.0f, 0.0f, -1.0f), seed, 2);
	//chunks[8] = new TerrainChunk(chunkSize, pos + glm::vec3(1.0f, 0.0f, -1.0f), seed, 2);
}

TerrainGenerator::~TerrainGenerator()
{
	//for (int i = 0; i < chunksVisible; i++)
	//{
	//	delete chunks[i];
	//	chunks[i] = NULL;
	//}
	//delete chunks;
}

void TerrainGenerator::update()
{
	// check cam position & see if chunks need updating
	viewerPos = getPosFromViewMatrix();

	// only use updateVisibleChunks when moving from one chunk to another
	updateVisibleChunks();

	// use updateChunksLOD can be modified when the camera has moved enough in one direction
}

void TerrainGenerator::render()
{
	for (listIterator = chunksLastVisible.begin(); listIterator != chunksLastVisible.end(); ++listIterator)
		(*listIterator)->render();
}

void TerrainGenerator::updateVisibleChunks()
{
	clearList();

	int currChunkCoordX = viewerPos.x / chunkSize;
	int currChunkCoordZ = viewerPos.z / chunkSize;
	char coordinateKey = 0;
	int lod = 3;
	for (int zOffset = -chunksVisible; zOffset <= chunksVisible; zOffset++)
	{
		for (int xOffset = -chunksVisible; xOffset <= chunksVisible; xOffset++)
		{
			glm::vec3 viewedChunkCoord(currChunkCoordX + xOffset, 0.0f, currChunkCoordZ + zOffset);
			coordinateKey = hash_vector(viewedChunkCoord);
			mapIterator = chunkMap->find(coordinateKey);
			if (mapIterator != chunkMap->end())
				updateChunkVisibility(mapIterator->second); // map already contains this chunk, so check if it is still visible
			else
				chunkMap->insert({ coordinateKey, new TerrainChunk(chunkSize, viewedChunkCoord, seed, lod)}); // chunk does not exist, create new chunk
		}
	}
}

void TerrainGenerator::updateChunkVisibility(TerrainChunk* chunk)
{
	float distance = getNearestChunkEdge(chunk);
	bool visible = distance <= maxViewDistance;
	chunk->setVisibility(visible);
	if (visible)
		chunksLastVisible.push_back(chunk);
}

void TerrainGenerator::clearList()
{
	for (listIterator = chunksLastVisible.begin(); listIterator != chunksLastVisible.end(); ++listIterator)
		(*listIterator)->setVisibility(false);
	chunksLastVisible.clear();
}

float TerrainGenerator::getNearestChunkEdge(TerrainChunk* chunk)
{
	float viewerDistance = 0.0f;
	glm::vec3 chunkPos = chunk->getPosition() * (float)chunk->getSize();
	float xMin = chunkPos.x, xMax = chunkPos.x + chunk->getSize(), yMin = chunkPos.z, yMax = chunkPos.z + chunk->getSize();
	float dx = glm::max(xMin - viewerPos.x, xMax - viewerPos.x);
	float dy = glm::max(yMin - viewerPos.z, yMax - viewerPos.z);
	viewerDistance = glm::sqrt(dx * dx + dy * dy);

	return viewerDistance;
}

glm::vec3 TerrainGenerator::getPosFromViewMatrix()
{
	glm::mat4 invView = glm::inverse(g_Camera3d->getViewMatrix());
	return glm::vec3(invView[3][0], invView[3][1], invView[3][2]);
}

char TerrainGenerator::hash_vector(glm::vec2 pos)
{
	char p = 0;
	if ((pos.x >= -3 && pos.x <= 3) && (pos.y >= -3 && pos.y <= 3))
	{
		int x = pos.x;
		int y = pos.y;
		bool nx = false, ny = false;
		nx = x < 0;
		ny = y < 0;
		char xx = glm::abs(x);
		char yy = glm::abs(y) << 2;
		p = xx | yy;
		if (nx) p = p | (char)32;
		if (ny) p = p | (char)64;
	}
	//else
	//	printf("\nError in hash_vector - position vector was out of range (-3,3)");
	return p;
}

char TerrainGenerator::hash_vector(glm::vec3 pos)
{
	return hash_vector(glm::vec2(pos.x, pos.z));
}
