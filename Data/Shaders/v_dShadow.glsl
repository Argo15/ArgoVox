#version 420

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
in vec3 v_vertex;

void main() {
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);
}