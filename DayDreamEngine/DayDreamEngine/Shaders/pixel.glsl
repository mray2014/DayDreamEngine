#version 430

layout (location = 4) uniform vec4 color;
layout (location = 5) uniform float time;

out vec4 fragColor;

void main() {
	fragColor = vec4(color.x * abs(sin(time)), color.y * abs(cos(time - 120.0f)), color.z * abs(sin(time - 240.0f)), 1.0f);
	//fragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}