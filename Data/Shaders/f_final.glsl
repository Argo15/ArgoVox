#version 420

uniform sampler2D colorTex;
uniform sampler2D lightTex;
uniform sampler2D glowTex;
in vec2 texCoord;
out vec4 finalBuffer;

void main() {
	vec4 color = texture2D(colorTex,texCoord);
	vec4 lightColor = texture2D(lightTex,texCoord);
	vec4 specColor = texture2D(glowTex,texCoord);
	finalBuffer = vec4(color.xyz*lightColor.xyz+specColor.xyz,1.0);	
} 