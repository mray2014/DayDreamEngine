#version 450
#include "ShaderStructs.h"		 // Holds all the structs to use between shaders
#include "VertexLayout.h"		 // Specifically for vertex shaders to layout vertex info
#include "MaterialUniformData.h" // If I want materials to use this shader

struct VertToPixelData{
	vec4 color;
	float time;
};

layout (location = VERT_LOCATION_OFFSET) out VertToPixelData data;

void main() {
	data.color = matInfo.color;
	data.time = constData.time;

	mat4 worldViewProj = constData.projMat * constData.viewMat * matInfo.worldMat;
	gl_Position = worldViewProj * vec4(pos, 1.0f);
}
