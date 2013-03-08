#version 420

uniform sampler2D bluringTex;
in vec2 texCoord;
out vec4 blurBuffer;

const float blurSize = 1.0/1280.0;

void main() {
	vec4 sum = vec4(0.0);
 
	vec4 pixelColor = texture(bluringTex, vec2(texCoord.x, texCoord.y));
   // blur in x (horizontal)
   // take nine samples, with the distance blurSize between them
   if (length(pixelColor) > 0.01)
   {
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y - 4.0*blurSize)) * 0.05;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y - 3.0*blurSize)) * 0.09;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y - 2.0*blurSize)) * 0.12;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y - blurSize)) * 0.15;
	   sum += pixelColor * 0.16;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + blurSize)) * 0.15;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + 2.0*blurSize)) * 0.12;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + 3.0*blurSize)) * 0.09;
	   sum += texture(bluringTex, vec2(texCoord.x, texCoord.y + 4.0*blurSize)) * 0.05;
   }
   else
   {
	   sum = pixelColor;
   }
 
   blurBuffer = sum;
} 