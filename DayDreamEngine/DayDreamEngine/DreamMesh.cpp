#include "DreamMesh.h"
#include "DreamGraphics.h"

DreamGraphics* graphics = nullptr;

DreamMesh::DreamMesh(std::vector<DreamVertex>& verts, std::vector<size_t>& indices) {
	graphics = DreamGraphics::GetInstance();
	
	if (graphics) {
		vertCount = verts.size();
		indicesCount = indices.size();

		size_t VBO; // Vertex Buffer

		graphics->GenerateVertexArray(1, vertexArray);
		graphics->GenerateBuffer(1, indexBuffer);
		graphics->GenerateBuffer(1, VBO);

		// Copying vertices
		graphics->BindBuffer(BufferType::ArrayBuffer, VBO);
		graphics->CopyBufferData(BufferType::ArrayBuffer, sizeof(DreamVertex) * vertCount, &verts[0], VertexDataUsage::StaticDraw);

		// Copying indices
		graphics->BindBuffer(BufferType::ElementArrayBuffer, indexBuffer);
		graphics->CopyBufferData(BufferType::ElementArrayBuffer, sizeof(size_t) * indicesCount, &indices[0], VertexDataUsage::StaticDraw);

		// Setting up how to read vertice data
		graphics->BindVertexArray(vertexArray);
		graphics->AddVertexAttributePointer(3, 0, false, sizeof(DreamVector3));
		graphics->UnBindVertexArray();
	}
}

void DreamMesh::DrawOpaque()
{
	graphics->BindVertexArray(vertexArray);

	if (indexBuffer != -1) {
		graphics->BindBuffer(BufferType::ElementArrayBuffer, indexBuffer);
		graphics->DrawWithIndex(indicesCount);
	}
	else {
		graphics->DrawWithVertex(vertCount);
	}
	
	graphics->UnBindVertexArray();
}
