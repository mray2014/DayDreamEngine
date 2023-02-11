#version 450
#include "DefaultShaderUniformData.h"

struct VertToPixelData{
	vec4 color;
	float time;
};

layout (location = 0) in vec3 pos;
layout (location = 1) out VertToPixelData data;

void main() {
	data.color = matInfo.color;
	data.time = constData.time;

	mat4 worldViewProj = constData.projMat * constData.viewMat * matInfo.worldMat;
	gl_Position = worldViewProj * vec4(pos, 1.0f);
}
