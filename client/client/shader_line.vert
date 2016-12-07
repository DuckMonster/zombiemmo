#version 330 core
in vec2 a_position;

uniform mat4 u_camera;

void main() {
	gl_Position = u_camera * vec4(a_position, 0.0, 1.0);
}