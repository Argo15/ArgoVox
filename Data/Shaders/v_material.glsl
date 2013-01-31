#version 420

uniform mat4 textureMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 inverseCameraMatrix;
uniform mat3 normalMatrix;
uniform bool curveGeometry;
in vec3 v_vertex;
in vec2 v_texture;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
out vec4 worldPos;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 texCoord;

void main() {
	worldPos = modelviewMatrix * inverseCameraMatrix * vec4(v_vertex,1.0);
	
	if (curveGeometry) {
		float radius = 100;
		vec3 center = vec3(0.0,-radius,0.0);
		vec2 direction = worldPos.xz;
		float distance = length(direction);
		direction = normalize(direction);
		float distFromCenter = radius+worldPos.y;
		float angle = distance/radius;  // trust me this is correct
		vec3 unitPosition = vec3(direction.x*sin(angle),cos(angle),direction.y*sin(angle));
		worldPos = vec4(center+unitPosition*distFromCenter,1.0);
	}
	
	gl_Position = projectionMatrix * modelviewMatrix * vec4(v_vertex,1.0);;
	texCoord = vec2(textureMatrix * vec4(v_texture,0.0,0.0));
	normal = normalMatrix * normalize(v_normal);
	tangent = normalMatrix * normalize(v_tangent);
	bitangent = normalMatrix * normalize(v_bitangent);
}