#version 450
#include "ShaderStructs.h"

layout(location = 0) out vec4 fragColor;

struct VertToPixelData{
	vec4 color;
	float time;
};
layout (location = VERT_LOCATION_OFFSET) in VertToPixelData data;

void main() {
	vec4 col = data.color;
	float t = data.time;

	fragColor = vec4(col.x * abs(sin(t)), col.y * abs(cos(t - 120.0f)), col.z * abs(sin(t - 240.0f)), 1.0f);
	//fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}