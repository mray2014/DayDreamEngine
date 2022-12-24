layout(location = 0) in vec3 pos;

void main() {
	gl_position = vec4(pos.x, pos.y, pos.z, 1.0f);
}