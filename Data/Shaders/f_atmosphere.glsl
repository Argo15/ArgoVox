#version 420

uniform sampler2D colorTex;
uniform sampler2D glowTex;
uniform sampler2D depthTex;
uniform sampler2D cloudTex;
uniform float near;
uniform float far;
uniform mat4 inverseMVPMatrix;
uniform vec3 cameraPos;
uniform vec3 sunDir; 
uniform vec3 nearSunColor;
uniform vec3 awaySunColor;
uniform vec3 cloudColor;
in vec2 texCoord;
out vec4 colorBuffer;
out vec4 glowBuffer;

void main() {
	float hyperDepth = texture2D(depthTex,texCoord).r;
	float depth = (2.0 * near) / (far + near - (hyperDepth*2.0-1.0) * (far - near));
	
	vec4 color = texture2D(colorTex,texCoord);
	vec4 glow = texture2D(glowTex,texCoord);
	if (depth>0.999) {
		vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
		vec4 worldPos = inverseMVPMatrix * screenPos;
		worldPos /= worldPos.w;
		vec3 eyeDir = normalize(worldPos.xyz-cameraPos.xyz);
		vec3 sun = normalize(-sunDir);
		float lightDotLook = dot(eyeDir,sun);
		lightDotLook = clamp(lightDotLook,0.0,1.0);
		float sunIntensity = pow(lightDotLook,120.0);
		//if (sunIntensity < 0.75) {
		//	sunIntensity = 0.0;
		//}
		float skyBrightness = 1-acos(dot(eyeDir,sun))/3.14;
		vec3 skyColor = skyBrightness*nearSunColor+(1.0-skyBrightness)*awaySunColor;
		
		
		eyeDir.y += 0.3;
		eyeDir = normalize(eyeDir);
		float cloudU = eyeDir.x;
		float cloudV = eyeDir.z;
		float factor = 2.0-abs(eyeDir.y);
		factor *= 0.4;
		float cloudValue = texture2D(cloudTex,factor*vec2(cloudU,cloudV)).r;
		
		color = vec4((1-cloudColor * cloudValue)*skyColor,color.w) + vec4(cloudColor * cloudValue,color.w);
		glow = glow + vec4(sunIntensity,sunIntensity,sunIntensity,0.0);
	}

	colorBuffer = color;	
	glowBuffer = glow;
} 