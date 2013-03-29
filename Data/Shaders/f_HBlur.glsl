#version 420

uniform sampler2D bluringTex;
in vec2 texCoord;
out vec4 blurBuffer;

const float blurSize = 1.0/1280.0;

float weights[9] = float[](
	0.05,
	0.09,
	0.12,
	0.15,
	0.16,
	0.15,
	0.12,
	0.09,
	0.05
);

void main() {
	vec4 sum = vec4(0.0);
	
	vec4 pixelColor = texture(bluringTex, vec2(texCoord.x, texCoord.y));
	// blur in x (horizontal)
	// take nine samples, with the distance blurSize between them
	if (length(pixelColor) > 0.01)
	{
		for (int i=0; i<9; i++)
		{
			sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + (i-4.0)*blurSize)) * weights[i];
		}
	}
	else
	{
		sum = pixelColor;
	}
 
	blurBuffer = sum;
} 