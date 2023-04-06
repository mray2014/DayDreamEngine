#version 450
#include "ShaderStructs.h"

layout(set = 0, binding = 2) uniform LightData {
	DirectionalLight light;
	float ambient;
}lightInfo;

layout (binding = 3) uniform sampler2D texSampler;

struct VertToPixelData{
	vec4 color;
	vec3 normal;
	float time;
	vec2 uv;
};
layout (location = VERT_LOCATION_OFFSET) in VertToPixelData data;
layout (location = 0) out vec4 fragColor;

void main() {
	float lighting = dot(lightInfo.light.lightDir, data.normal);

	vec4 color  = texture(texSampler, data.uv);
	//color *= lighting;

	fragColor = color;
}