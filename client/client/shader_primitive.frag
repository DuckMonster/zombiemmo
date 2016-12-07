#version 330 core

struct material {
	vec4 color;
};

uniform material u_material;
out vec4 o_color;

void main() {
	o_color = u_material.color;
}