#include "DreamGameObject.h"

DreamGameObject::DreamGameObject()
{
	transform = DreamMath::DreamTransform();
	materialInstanceComp = MatDataComponent();
	objMesh = nullptr;
	material = nullptr;
}
DreamGameObject::DreamGameObject(DreamMesh* mesh, DreamMaterial* mat) {
	transform = DreamMath::DreamTransform();
	materialInstanceComp = MatDataComponent();
	objMesh = mesh;
	material = mat;
}

void DreamGameObject::Draw()
{
	materialInstanceComp.worldMat = transform.GetWorldMatrix();
	if (material && objMesh) {
		material->Bind(materialInstanceComp);
		objMesh->DrawOpaque();
	}
}
