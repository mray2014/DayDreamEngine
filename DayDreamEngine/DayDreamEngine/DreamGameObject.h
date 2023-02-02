#pragma once
#include <DreamMath.h>

class DreamGameObject {
public:

	DreamGameObject();
	virtual ~DreamGameObject() {};

	DreamMath::DreamTransform transform;

	virtual void Update() {};
private:

};