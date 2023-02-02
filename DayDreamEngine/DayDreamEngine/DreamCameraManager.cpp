#include "DreamCameraManager.h"



DreamCameraManager* DreamCameraManager::camManager = nullptr;

DreamCameraManager* DreamCameraManager::GetInstance()
{
	if (!camManager) {
		camManager = new DreamCameraManager();
	}

	return camManager;
}

void DreamCameraManager::SetActiveCamera(DreamCamera* cam)
{
	currentCamera = cam;
}

DreamMath::DreamMatrix4X4 DreamCameraManager::GetCurrentCam_ViewMat()
{
	if (currentCamera) {
		return currentCamera->viewMat;
	}
	return DreamMath::DreamMatrix4X4();
}

DreamMath::DreamMatrix4X4 DreamCameraManager::GetCurrentCam_ProjMat()
{
	if (currentCamera) {
		return currentCamera->projectionMat;
	}
	return DreamMath::DreamMatrix4X4();
}
