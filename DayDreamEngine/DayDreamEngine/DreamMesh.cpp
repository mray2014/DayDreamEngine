#include "DreamMesh.h"
#include "DreamGraphics.h"

DreamGraphics* graphics = nullptr;

DreamMesh::DreamMesh(std::vector<DreamVertex>& verts) {
	graphics = DreamGraphics::GetInstance();

	if (graphics) {
		vertCount = verts.size();

		size_t VBO; // Vertex Buffer

		graphics->GenerateBuffer(BufferType::VertexArray, vertexArray);
		graphics->GenerateBuffer(BufferType::ArrayBuffer, VBO, 1, &verts[0], sizeof(DreamVertex) * vertCount);

		// Copying vertices

		// Setting up how to read vertice data
		graphics->BindBuffer(BufferType::VertexArray, vertexArray);
		graphics->AddVertexAttributePointer(3, 0, false, sizeof(DreamVector3));
		graphics->UnBindBuffer(BufferType::VertexArray);
	}
}


DreamMesh::DreamMesh(std::vector<DreamVertex>& verts, std::vector<size_t>& indices) {
	graphics = DreamGraphics::GetInstance();
	
	if (graphics) {
		vertCount = verts.size();
		indicesCount = indices.size();

		size_t VBO; // Vertex Buffer

		graphics->GenerateBuffer(BufferType::VertexArray, vertexArray);
		graphics->GenerateBuffer(BufferType::ElementArrayBuffer, indexBuffer, 1, &indices[0], sizeof(size_t) * indicesCount);
		graphics->GenerateBuffer(BufferType::ArrayBuffer, VBO, 1, &verts[0], sizeof(DreamVertex) * vertCount);

		// Copying vertices
		//graphics->BindBuffer(BufferType::ArrayBuffer, VBO);

		// Copying indices
		//graphics->BindBuffer(BufferType::ElementArrayBuffer, indexBuffer);
		//graphics->CopyBufferData(BufferType::ElementArrayBuffer, , , VertexDataUsage::StaticDraw);

		// Setting up how to read vertice data
		graphics->BindBuffer(BufferType::VertexArray, vertexArray);
		graphics->AddVertexAttributePointer(3, 0, false, sizeof(DreamVector3));
		graphics->UnBindBuffer(BufferType::VertexArray);
	}
}

void DreamMesh::DrawOpaque()
{
	graphics->BindBuffer(BufferType::VertexArray, vertexArray);

	if (indexBuffer != -1) {
		graphics->BindBuffer(BufferType::ElementArrayBuffer, indexBuffer);
		graphics->DrawWithIndex(indicesCount);
	}
	else {
		graphics->DrawWithVertex(vertCount);
	}
	
	graphics->UnBindBuffer(BufferType::VertexArray);
}
