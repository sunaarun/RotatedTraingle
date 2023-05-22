#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 matrix; // our matrix
uniform mat4 matT1; // our matrix
uniform mat4 matT2; // our matrix

out vec3 colour;

void main() {
	colour = vertex_colour;
	gl_Position = matT2* matrix * matT1* vec4(vertex_position, 1.0);
}