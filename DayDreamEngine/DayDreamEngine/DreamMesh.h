#pragma once
#include <vector>
#include <DreamMath.h>
#include "DreamShader.h"

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

	DreamShaderLinker* shaderLink = nullptr;

public:

	DreamMesh(DreamShaderLinker* shader, std::vector<DreamVertex>& verts);
	DreamMesh(DreamShaderLinker* shader, std::vector<DreamVertex>& verts, std::vector<size_t>& indices);
	~DreamMesh();

	void DrawOpaque();
};
