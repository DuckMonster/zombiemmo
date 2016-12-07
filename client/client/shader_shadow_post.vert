#version 330 core
in vec2 a_vertex;
in vec2 a_uv;

out vec2 f_uv;

void main() {
	gl_Position = vec4(a_vertex, 0.0, 1.0);
	f_uv = a_uv;
}