#version 420

uniform mat4 textureMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 projectionCameraMatrix;
uniform mat4 projectionLastCameraMatrix;
uniform mat3 normalMatrix;
uniform vec3 camPos;
uniform vec3 lastCamPos;
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
out vec4 newPosition;
out vec4 lastPosition;

vec4 CurvedPosition(vec3 cameraPosition, vec4 worldPosition, float planetRadius) {
	vec3 center = vec3(cameraPosition.x,-planetRadius,cameraPosition.z);
	vec2 direction = worldPosition.xz-cameraPosition.xz;
	float distance = length(direction);
	direction = normalize(direction);
	float distFromCenter = planetRadius+worldPosition.y;
	float angle = distance/planetRadius;  // arclength/radius = angle
	vec3 unitPosition = vec3(direction.x*sin(angle),cos(angle),direction.y*sin(angle));
	return vec4(center+unitPosition*distFromCenter,1.0);
}

void main() {
	worldPos = modelviewMatrix * vec4(v_vertex,1.0);
	vec4 curvedWorldPos = CurvedPosition(camPos, worldPos, 100);
	vec4 curvedLastWorldPos = CurvedPosition(lastCamPos, worldPos, 100);
	
	newPosition = projectionCameraMatrix * curvedWorldPos;
	lastPosition = projectionLastCameraMatrix * curvedLastWorldPos;
	
	gl_Position = newPosition;
	texCoord = vec2(textureMatrix * vec4(v_texture,0.0,0.0));
	normal = normalMatrix * normalize(v_normal);
	tangent = normalMatrix * normalize(v_tangent);
	bitangent = normalMatrix * normalize(v_bitangent);
}