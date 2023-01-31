#pragma once
#include "DreamGameObject.h"

class DreamCamera : public DreamGameObject{

public:
	DreamCamera();
	~DreamCamera() override;

	virtual void Update() override;


	float cameraSpeed = 1.0f;

	DreamMath::DreamMatrix4X4 viewMat;
	DreamMath::DreamMatrix4X4 ProjectionMat;
private:


};

