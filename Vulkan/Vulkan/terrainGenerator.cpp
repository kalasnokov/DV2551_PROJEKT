#include "pch.h"
#include "terrainGenerator.hpp"



void terrainGenerator::setUp(dataObjects * dataObjects)
{
	dataObjectptr = dataObjects;

}

terrainGenerator::terrainGenerator() {}


terrainGenerator::~terrainGenerator()
{
	//Unmap memory
}
