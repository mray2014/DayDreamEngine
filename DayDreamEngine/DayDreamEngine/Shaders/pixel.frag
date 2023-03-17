#version 450
#include "ShaderStructs.h"

layout(set = 0, binding = 2) uniform LightData {
	DirectionalLight light;
	float ambient;
}lightInfo;

struct VertToPixelData{
	vec4 color;
	vec3 normal;
	float time;
	vec2 uv;
};
layout (location = VERT_LOCATION_OFFSET) in VertToPixelData data;
layout (location = 0) out vec4 fragColor;

void main() {
	vec4 col = data.color;
	float t = data.time;

	float lighting = dot(lightInfo.light.lightDir, data.normal);
	//lighting *= lightInfo.ambient;

	vec3 color = vec3(col.x * abs(sin(t)), col.y * abs(cos(t - 120.0f)), col.z * abs(sin(t - 240.0f))) * lightInfo.light.lightColor;

	color *= lighting;

	fragColor = vec4(color, 1.0f);
	//fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}