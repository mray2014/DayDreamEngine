
// All shaders should have these uniform buffers included
layout(set = 0, binding = 0) uniform ConstantData {
	mat4 viewMat;
	mat4 projMat;
	float time;
}constData;

layout(set = 0, binding = 1) uniform MaterialData {
	mat4 worldMat;
	vec4 color;
	float specular;
}matInfo;