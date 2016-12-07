#version 330 core
in ivec2 a_vertex;

uniform mat4 u_camera;

void main() {
	gl_Position = u_camera * vec4(a_vertex, 0.0, 1.0);
}