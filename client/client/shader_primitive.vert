#version 330 core
in ivec2 a_vertex;

uniform mat4 u_camera;
uniform mat4 u_model;

void main() {
	gl_Position = u_camera * u_model * vec4(a_vertex, 0.0, 1.0);
}