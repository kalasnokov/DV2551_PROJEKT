#include "pch.h"
#include "mesh.h"


Mesh::Mesh() {
}

void Mesh::loadTerrainGenOutput(int pos, int chunkSize) {

}

void Mesh::makeSimpleMesh(int dim) {
	for (int x = 0; x < dim; x++) {
		for (int y = 0; y < dim; y++) {
			vertices.push_back({{x, y, 0},{1, 0, 0}});
		}
	}

	int quads = dim - 1;
	for (int i = 0; i < quads; i++) {
		int yPos = dim * i;
		for (int y = 0; y < quads; y++) {
			indices.push_back(yPos + y);
			indices.push_back(yPos + dim + y);
			indices.push_back(yPos + dim + y + 1);
			indices.push_back(yPos + dim + y + 1);
			indices.push_back(yPos + y + 1);
			indices.push_back(yPos + y);
		}
	}
}

std::vector<Vertex> Mesh::getVertices() {
	return vertices;
}

std::vector<uint32_t> Mesh::getIndices() {
	return indices;
}

Mesh::~Mesh() {
}
