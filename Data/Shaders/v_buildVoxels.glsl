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

out VertexData {
	vec3 worldPos;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec2 texCoord;
} VertexOut;

void main() {
	VertexOut.worldPos = vec3(invCameraMatrix * modelviewMatrix * vec4(v_vertex,1.0));
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);
	VertexOut.texCoord = vec2(textureMatrix * vec4(v_texture,0.0,0.0));
	VertexOut.normal = normalMatrix * normalize(v_normal);
	VertexOut.tangent = normalMatrix * normalize(v_tangent);
	VertexOut.bitangent = normalMatrix * normalize(v_bitangent);
}