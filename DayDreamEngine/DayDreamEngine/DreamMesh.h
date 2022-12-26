#pragma once
#include <DreamMath.h>
#include <vector>

using namespace DreamMath;

struct DreamVertex {
	DreamVector3 pos;

	DreamVertex(DreamVector3 p) {
		pos = p;
	}
	DreamVertex(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}
};
class DreamMesh {
	int vertCount = 0;
	int indicesCount = 0;
	size_t indexBuffer = -1; // Index Buffer
	size_t vertexArray = -1; // Vertex Array

public:

	DreamMesh(std::vector<DreamVertex>& verts);
	DreamMesh(std::vector<DreamVertex>& verts, std::vector<size_t>& indices);
	void DrawOpaque();
};
