#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) uniform mat4 modelWorld;
layout (location = 2) uniform mat4 view;
layout (location = 3) uniform mat4 proj;

void main() {
	mat4 worldViewProj = proj * view * modelWorld;
	gl_Position = worldViewProj * vec4(pos, 1.0f);
	//gl_Position = vec4(pos, 1.0f);
}