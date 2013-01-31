#version 420

uniform sampler2D tex;
uniform sampler2D velTex;
uniform float numSamples;
in vec2 texCoord;
out vec4 blurBuffer;

void main() {
	vec4 totalColor = vec4(0.0);
	vec2 velocity = texture2D(velTex,texCoord.st).rg*2.0-vec2(1.0);
	vec2 vel = velocity;
	velocity /= 3.25;
	for (float i = 0; i<numSamples; i++) {
		totalColor += texture2D(tex,texCoord.st-velocity*((i-(numSamples/2))/numSamples));
	}
	totalColor /= numSamples;
	if (length(vel) > 0.99)
		totalColor = texture2D(tex,texCoord.st);
	blurBuffer = vec4(totalColor.xyz,1.0);	
} 