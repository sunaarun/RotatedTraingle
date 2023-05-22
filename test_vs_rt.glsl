#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 matrix2; // our matrix
uniform mat4 matT12; // our matrix
uniform mat4 matT22; // our matrix

out vec3 colour;

void main() {
	colour = vertex_colour;
	gl_Position = matT22* matrix2 * matT12* vec4(vertex_position, 1.0);
}