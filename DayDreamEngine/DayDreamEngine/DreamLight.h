#pragma once
#include <DreamMath.h>

struct DreamDirectionalLight {
	DreamMath::DreamVector3 direction;
	float pad = 0;
	DreamMath::DreamVector3 color;
	float pad2 = 0;
	DreamDirectionalLight() {

	}
	DreamDirectionalLight(DreamMath::DreamVector3 dir, DreamMath::DreamVector3 col = DreamMath::DreamVector3(1, 1, 1)) {
		direction = dir; 
		color = col;
	}
};

struct DreamSpotLight {
	DreamMath::DreamVector3 position;
	DreamMath::DreamVector3 color;
	float size;
};