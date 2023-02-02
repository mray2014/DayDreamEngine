#pragma once
#include "DreamGameObject.h"

class DreamCamera : public DreamGameObject{

public:
	DreamCamera();
	~DreamCamera() override;

	virtual void Update() override;

	bool isClicking = false;

	float cameraSpeed = 1.0f;


	float fieldOfView = 90.0f;
	float nearClipDist = 0.1f;
	float farClipDist = 1000.0f;

	DreamMath::DreamMatrix4X4 viewMat;
	DreamMath::DreamMatrix4X4 projectionMat;
private:


};

