#version 420

uniform mat4 projectionMatrix;
in vec3 v_vertex;
in vec2 v_texture;
out vec2 texCoord;

void main() {
	texCoord = v_texture;
	gl_Position = projectionMatrix * vec4(v_vertex,1.0);
}