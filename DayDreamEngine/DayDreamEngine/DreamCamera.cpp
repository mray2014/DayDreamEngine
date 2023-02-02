#include "DreamCamera.h"
#include <DreamInput.h>
#include <DreamTimeManager.h>
#include "DreamGraphics.h"

DreamCamera::DreamCamera() : DreamGameObject()
{
}

DreamCamera::~DreamCamera()
{
}

void DreamCamera::Update()
{
	float deltaTime = DreamTimeManager::deltaTime;

 	if (DreamInput::KeyDown(KeyCode::W)) {
		transform.position += (transform.GetForward() * cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::A)) {
		transform.position += (transform.GetRight() * -cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::S)) {
		transform.position += (transform.GetForward() * -cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::D)) {
		transform.position += (transform.GetRight() * cameraSpeed) * deltaTime;
	}

	float aspectratio = DreamGraphics::GetAspectRatio();

	// Perspective matrix
	// Update our projection matrix since the window size changed
	//XMMATRIX P = XMMatrixPerspectiveFovLH(
	//	0.25f * 3.1415926535f,	// Field of View Angle
	//	(float)width / height,	// Aspect ratio
	//	0.1f,				  	// Near clip plane distance
	//	100.0f);			  	// Far clip plane distance
	//XMStoreFloat4x4(&cam->projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

}
