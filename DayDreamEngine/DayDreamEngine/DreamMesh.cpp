#include "DreamMesh.h"
#include "DreamGraphics.h"

DreamGraphics* graphics = nullptr;

DreamMesh::DreamMesh(std::vector<DreamVertex>& verts) {
	graphics = DreamGraphics::GetInstance();

	if (graphics) {
		vertCount = verts.size();

		DreamBuffer* vertBuffer = graphics->GenerateBuffer(BufferType::ArrayBuffer, &verts[0], vertCount, { sizeof(DreamVertex) });
		
		vertArray = graphics->GenerateVertexArray(vertBuffer);
	}
}


DreamMesh::DreamMesh(std::vector<DreamVertex>& verts, std::vector<uint32_t>& indices) {
	graphics = DreamGraphics::GetInstance();

	if (graphics) {
		vertCount = verts.size();
		indicesCount = indices.size();

		DreamBuffer* vertBuffer = graphics->GenerateBuffer(BufferType::ArrayBuffer, &verts[0], vertCount, { sizeof(DreamVertex) });
		DreamBuffer* indexBuffer = graphics->GenerateBuffer(BufferType::ElementArrayBuffer, &indices[0], indicesCount, { sizeof(uint32_t) });
		
		vertArray = graphics->GenerateVertexArray(vertBuffer, indexBuffer);
	}
}

DreamMesh::~DreamMesh()
{
	if (vertArray) {
		delete vertArray;
		vertArray = nullptr;
	}
}

//TODO: Change the name of this function, move to a different place?
void DreamMesh::DrawOpaque()
{
	vertArray->Bind();	

	if (vertArray->indexBuffer) {
		graphics->DrawWithIndex(indicesCount);
	}
	else {
		graphics->DrawWithVertex(vertCount);
	}

	vertArray->UnBind();
}
