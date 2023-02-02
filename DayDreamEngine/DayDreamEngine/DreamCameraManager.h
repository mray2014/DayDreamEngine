#pragma once
#include "DreamCamera.h"

class DreamCameraManager {
public:

	static DreamCameraManager* GetInstance();

	void SetActiveCamera(DreamCamera* cam);

	DreamMath::DreamMatrix4X4 GetCurrentCam_ViewMat();
	DreamMath::DreamMatrix4X4 GetCurrentCam_ProjMat();


	DreamCamera* currentCamera = nullptr;

private:
	static DreamCameraManager* camManager;

};
