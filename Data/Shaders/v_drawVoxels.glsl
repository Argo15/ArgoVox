#version 420

uniform mat4 modelviewMatrix;
uniform mat4 invCameraMatrix;
uniform mat4 projectionMatrix;
uniform layout ( binding = 3, r32ui ) coherent volatile uimage3D voxelmap;
in vec3 v_vertex;
out vec3 worldPos;

uint voxelGridSize = 16;
uint numVoxels = 32;

void main() {
	worldPos = vec3(invCameraMatrix * modelviewMatrix * vec4(v_vertex,1.0));
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);
}