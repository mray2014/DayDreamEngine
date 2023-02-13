#pragma once
#include <vector>
#include <DreamMath.h>
#include "DreamMaterial.h"

class DreamShaderLinker;
class DreamVertexArray;
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

	DreamVertexArray* vertArray;



public:

	DreamMesh(std::vector<DreamVertex>& verts);
	DreamMesh(std::vector<DreamVertex>& verts, std::vector<uint32_t>& indices);
	~DreamMesh();

	void DrawOpaque();
};
