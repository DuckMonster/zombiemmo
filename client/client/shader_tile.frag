#version 330 core

struct global {
	vec4 foreground;
	vec4 background;
};

in VS_OUT {
	vec2 uv;
	vec2 index;
	vec4 foreground;
	vec4 background;
} fs_in;

uniform sampler2D u_sampler;
uniform ivec2 u_tileSize;
uniform global u_global;

out vec4 o_color;

void main() {
	vec2 uv = fs_in.uv;
	uv = (uv + fs_in.index) / textureSize(u_sampler, 0) * u_tileSize;

	o_color = texture(u_sampler, uv) * fs_in.foreground * u_global.foreground;

	if (o_color.a <= 0.0) {
		if (fs_in.background.a > 0.0 && u_global.background.a > 0.0)
			o_color = fs_in.background * u_global.background;
		else
			discard;
	}
}