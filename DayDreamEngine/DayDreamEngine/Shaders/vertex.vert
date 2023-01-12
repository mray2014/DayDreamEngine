#version 450

layout(location = 0) in vec3 pos;

vec3 positions[3] = vec3[](
    vec3(0.0, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),
    vec3(-0.5, 0.5, 0.0)
);

void main() {
    gl_Position = vec4(pos, 1.0f);
	//gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
}