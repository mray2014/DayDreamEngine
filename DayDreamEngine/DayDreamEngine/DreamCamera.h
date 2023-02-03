#pragma once
#include "DreamGameObject.h"

class DreamCamera : public DreamGameObject{

public:
	DreamCamera();
	~DreamCamera() override;

	virtual void Update() override;

	bool isClicking = false;
	DreamMath::DreamVector2 mouseStartPos;
	DreamMath::DreamVector2 mouseCurPos;

	float cameraSpeed = 2.0f;
	float cameraSpeedMultiplier = 1.0f;
	float cameraSpeedMaxMultiplier = 2.0f;
	float cameraRotSpeed = 20.0f;

	bool instantZoom = false;
	float zoomSpeed = 15.0f;
	float zoom = 1.0f;
	float minZoom = 1.0f;
	float maxZoom = 3.0f;

	float fieldOfView = 90.0f;
	float nearClipDist = 0.1f;
	float farClipDist = 1000.0f;

	DreamMath::DreamMatrix4X4 viewMat;
	DreamMath::DreamMatrix4X4 projectionMat;
private:


};

