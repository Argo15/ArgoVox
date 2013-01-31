#version 420

uniform mat4 projectionCameraMatrix;
uniform mat4 modelviewMatrix;
uniform vec3 camPos;
in vec3 v_vertex;

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
	vec4 worldPos = modelviewMatrix * vec4(v_vertex,1.0);
	vec4 curvedWorldPos = CurvedPosition(camPos, worldPos, 100);
	gl_Position = projectionCameraMatrix * curvedWorldPos;
}