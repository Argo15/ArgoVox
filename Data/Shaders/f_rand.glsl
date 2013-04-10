#version 420

in vec2 texCoord;
out vec4 randBuffer;

void main() {
	float random = 0.5 + 0.5 * fract(sin(dot(texCoord.xy, vec2(12.9898, 78.233)))* 43758.5453);
	randBuffer = vec4(random);
} 