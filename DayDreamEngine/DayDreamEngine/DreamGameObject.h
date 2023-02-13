#pragma once
#include <DreamMath.h>
#include "DreamMesh.h"

class DreamGameObject {
public:

	DreamGameObject();
	DreamGameObject(DreamMesh* mesh, DreamMaterial* mat);
	virtual ~DreamGameObject() {};

	void Draw();

	DreamMath::DreamTransform transform;
	MatDataComponent materialInstanceComp;
	DreamMaterial* material;
	DreamMesh* objMesh;

	virtual void Update() {};
private:

};