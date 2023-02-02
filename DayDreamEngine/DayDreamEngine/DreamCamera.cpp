#include "DreamCamera.h"
#include <DreamInput.h>
#include <DreamTimeManager.h>
#include "DreamGraphics.h"

DreamCamera::DreamCamera() : DreamGameObject()
{
	transform.position.z = 1.0f;
}

DreamCamera::~DreamCamera()
{
}

void DreamCamera::Update()
{
	float deltaTime = DreamTimeManager::deltaTime;

 	if (DreamInput::KeyDown(KeyCode::W)) {
		transform.position += (transform.GetForward() * -cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::A)) {
		transform.position += (transform.GetRight() * -cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::S)) {
		transform.position += (transform.GetForward() * cameraSpeed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::D)) {
		transform.position += (transform.GetRight() * cameraSpeed) * deltaTime;
	}

	if (DreamInput::KeyDown(KeyCode::Q)) {
		fieldOfView -= 30.0f * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::E)) {
		fieldOfView += 30.0f * deltaTime;
	}


	if (DreamInput::KeyDown(KeyCode::MOUSE_LEFT_CLICK)) {
		transform.Rotate(DreamMath::DreamVector3(0.0f, 60.0f * deltaTime, 0.0f));
	}

	if (DreamInput::KeyDown(KeyCode::MOUSE_RIGHT_CLICK)) {
		transform.Rotate(DreamMath::DreamVector3(60.0f * deltaTime, 0.0f, 0.0f));
	}

	float aspectratio = DreamGraphics::GetAspectRatio();

	projectionMat = DreamMath::CreateProjectionMatix(fieldOfView, nearClipDist, farClipDist);
	//viewMat = DreamMath::LookAtViewMatix(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	viewMat = transform.GetWorldMatrix();
	//viewMat.matrix.data[3][3] = 0.0f;

	// Perspective matrix
	// Update our projection matrix since the window size changed
	//XMMATRIX P = XMMatrixPerspectiveFovLH(
	//	0.25f * 3.1415926535f,	// Field of View Angle
	//	(float)width / height,	// Aspect ratio
	//	0.1f,				  	// Near clip plane distance
	//	100.0f);			  	// Far clip plane distance
	//XMStoreFloat4x4(&cam->projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

}
