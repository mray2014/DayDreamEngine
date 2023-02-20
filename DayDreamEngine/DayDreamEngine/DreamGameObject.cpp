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

	material->LoadUniformIndexs(indexStore);
}

void DreamGameObject::Draw()
{
	materialInstanceComp.worldMat = transform.GetWorldMatrix();
	if (material && objMesh) {
		material->UpdateUniformData<MatDataComponent>(materialInstanceComp, "MaterialData", indexStore["MaterialData"]);
		material->Bind(indexStore);
		objMesh->DrawOpaque();
	}
}
