#version 420

uniform mat4 modelviewMatrix;
uniform mat4 invCameraMatrix;
uniform mat4 projectionMatrix;
in vec3 v_vertex;
out vec3 worldPos;

void main() {
	worldPos = vec3(invCameraMatrix * modelviewMatrix * vec4(v_vertex,1.0));
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);
}