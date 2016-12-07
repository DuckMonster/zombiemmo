#version 330 core

in vec2 f_uv;

uniform sampler2D sampler;
uniform bool u_grayscale;
uniform vec4 u_tint;

out vec4 o_color;

void main() {
	o_color = texture(sampler, f_uv);

	// Grayscale by weighting the different values
	if (u_grayscale) {
		float g = o_color.r * 0.299 + o_color.g * 0.587 + o_color.b * 0.114;
		o_color.xyz = vec3(g, g, g) * 0.4;
		o_color.xyz = mix( o_color.xyz, u_tint.xyz, u_tint.a );
	}
}