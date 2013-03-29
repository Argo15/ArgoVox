#version 420

uniform sampler2D bluringTex;
uniform sampler2D normalTex;
in vec2 texCoord;
out vec4 blurBuffer;

const float blurSize = 1.0/1280.0;

float weights[5] = float[](
	0.12,
	0.15,
	0.16,
	0.15,
	0.12
);

void main() {
	vec4 sum = vec4(0.0);
	
	vec4 pixelColor = texture(bluringTex, vec2(texCoord.x, texCoord.y));
	vec3 pixelNormal = normalize(texture(normalTex, vec2(texCoord.x, texCoord.y)).xyz*vec3(2.0)-vec3(1.0));
	// blur in x (horizontal)
	// take nine samples, with the distance blurSize between them
	if (length(pixelColor) > 0.01)
	{
		float totalWeight = 0;
		for (int i=0; i<5; i++)
		{
			vec3 sampleNormal = normalize(texture(normalTex, vec2(texCoord.x, texCoord.y + (i-2.0)*blurSize)).xyz*vec3(2.0)-vec3(1.0));
			float normalDot = dot(sampleNormal, pixelNormal);
			if (normalDot >= 0.8)
			{
				sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + (i-2.0)*blurSize)) * weights[i];
				totalWeight += weights[i];
			}
		}
		sum /= totalWeight;
	}
	else
	{
		sum = pixelColor;
	}
 
	blurBuffer = sum;
} 