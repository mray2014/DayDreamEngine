#include "DreamCamera.h"
#include <DreamInput.h>
#include <DreamTimeManager.h>
#include "DreamGraphics.h"

DreamCamera::DreamCamera() : DreamGameObject()
{
	transform.position.z = 1.0f;
	transform.SetRotation(DreamMath::DreamVector3(0.0f, 180.0f, 0.0f));
	//transform.SetForward(DreamMath::DreamVector3(0.0f, 0.0f, -1.0f)); //TODO: immediately breaks
}

DreamCamera::~DreamCamera()
{
}

void DreamCamera::Update()
{
	float deltaTime = DreamTimeManager::deltaTime;
	float speed = cameraSpeed * cameraSpeedMultiplier;

	// Movement Controls
 	if (DreamInput::KeyDown(KeyCode::W)) {
		transform.position += (transform.GetForward() * speed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::A)) {
		transform.position += (transform.GetRight() * -speed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::S)) {
		transform.position += (transform.GetForward() * -speed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::D)) {
		transform.position += (transform.GetRight() * speed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::SPACE)) {
		transform.position += (DreamVector3(0.0f, 1.0f, 0.0f) * speed) * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::CTRL)) {
		transform.position -= (DreamVector3(0.0f, 1.0f, 0.0f) * speed) * deltaTime;
	}

	if (DreamInput::KeyDown(KeyCode::SHIFT)) {
		cameraSpeedMultiplier = cameraSpeedMaxMultiplier;
	}
	else {
		cameraSpeedMultiplier = 1.0f;
	}

	// FoV Controls
	if (DreamInput::KeyDown(KeyCode::Q)) {
		fieldOfView -= 30.0f * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::E)) {
		fieldOfView += 30.0f * deltaTime;
	}

	// Zoom Controls
	if (DreamInput::KeyDown(KeyCode::Z)) {
		zoom -= 1.0f * deltaTime;
	}
	if (DreamInput::KeyDown(KeyCode::C)) {
		zoom += 1.0f * deltaTime;
	}

	// Cam Rotation Controls
	if (DreamInput::KeyPressed(KeyCode::MOUSE_LEFT_CLICK)) {
		isClicking = true;
		mouseStartPos = DreamInput::GetMousePosScreen();
	}
	if (DreamInput::KeyUp(KeyCode::MOUSE_LEFT_CLICK)) {
		isClicking = false;
	}

	// Cam Zoom Controls
	if (DreamInput::KeyDown(KeyCode::MOUSE_RIGHT_CLICK)) {
		if (instantZoom) {
			zoom = maxZoom;
		}
		else {
			if (zoom != maxZoom) {
				zoom += zoomSpeed * deltaTime;
				if (zoom >= maxZoom) {
					zoom = maxZoom;
				}
			}
		}	
	}
	else {
		if (instantZoom) {
			zoom = minZoom;
		}
		else {
			if (zoom != minZoom) {
				zoom -= zoomSpeed * deltaTime;
				if (zoom <= minZoom) {
					zoom = minZoom;
				}
			}
		}
	}
	
	if (isClicking) {
		mouseCurPos = DreamInput::GetMousePosScreen();
		DreamMath::DreamVector2 distDragged = mouseCurPos - mouseStartPos;
		mouseStartPos = mouseCurPos;

		distDragged *= deltaTime * cameraRotSpeed;

		transform.Rotate(DreamMath::DreamVector3(distDragged.y, -distDragged.x, 0.0f));
	}


	float aspectratio = DreamGraphics::GetAspectRatio();

	projectionMat = DreamMath::CreateProjectionMatix(fieldOfView, nearClipDist, farClipDist, aspectratio, zoom);
	viewMat = DreamMath::LookAtViewMatix(transform.position, transform.position + transform.GetForward(), transform.GetUp());

	// Perspective matrix
	// Update our projection matrix since the window size changed
	//XMMATRIX P = XMMatrixPerspectiveFovLH(
	//	0.25f * 3.1415926535f,	// Field of View Angle
	//	(float)width / height,	// Aspect ratio
	//	0.1f,				  	// Near clip plane distance
	//	100.0f);			  	// Far clip plane distance
	//XMStoreFloat4x4(&cam->projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

}
