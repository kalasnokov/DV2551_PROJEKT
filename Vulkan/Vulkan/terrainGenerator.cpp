#include "pch.h"
#include "terrainGenerator.hpp"



void terrainGenerator::setUp(VHF::dataObjects * dataObjects)
{
	dataObjectptr = dataObjects;
}

terrainGenerator::terrainGenerator() {}


terrainGenerator::~terrainGenerator()
{
	//Unmap memory
}
