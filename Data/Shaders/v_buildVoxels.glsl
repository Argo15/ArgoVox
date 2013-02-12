#version 420

uniform mat4 textureMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 invCameraMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
in vec3 v_vertex;
in vec2 v_texture;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
out vec3 worldPos;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 texCoord;

void main() {
	worldPos = vec3(invCameraMatrix * modelviewMatrix * vec4(v_vertex,1.0));
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);
	texCoord = vec2(textureMatrix * vec4(v_texture,0.0,0.0));
	normal = normalMatrix * normalize(v_normal);
	tangent = normalMatrix * normalize(v_tangent);
	bitangent = normalMatrix * normalize(v_bitangent);
}