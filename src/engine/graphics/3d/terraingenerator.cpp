#include "terraingenerator.h"
#include <random>

TerrainGenerator::TerrainGenerator() 
{
	shader = new Shader("res/shaders/heightmap.vert", "res/shaders/heightmap.frag");
}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::render()
{
}