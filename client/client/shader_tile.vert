#version 330 core

in vec2 a_vertex;
in vec2 a_uv;

in ivec2 a_offset;
in ivec2 a_index;
in vec4 a_foreground;
in vec4 a_background;

uniform float u_depth;
uniform mat4 u_camera;
uniform ivec2 u_offset;
uniform ivec2 u_tileSize;

out VS_OUT {
	vec2 uv;
	vec2 index;
	vec4 foreground;
	vec4 background;
} vs_out;

void main() {
	gl_Position = u_camera * vec4((a_vertex + a_offset + u_offset), u_depth, 1.0);

	vs_out.uv = a_uv;
	vs_out.index = a_index;
	vs_out.foreground = a_foreground;
	vs_out.background = a_background;
}