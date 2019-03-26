#pragma once
#include "dataObjects.hpp"
class Mesh
{
public:
	Mesh();

	void loadTerrainGenOutput(int pos, int chunkSize);

	void makeSimpleMesh(int dim);
	std::vector<Vertex> getVertices();
	std::vector<uint32_t> getIndices();
	~Mesh();

private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

