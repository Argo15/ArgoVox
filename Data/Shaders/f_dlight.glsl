#version 420

struct DirectLight
{
  vec3 direction;
  vec3 color;
  float specular;
  float ambient;
  float diffuse;
};

uniform mat4 inverseMVPMatrix;
uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D glowTex;
uniform sampler2DShadow shadowMap[4];
uniform float slices[3];
uniform mat4 lightMatrix[4];
uniform DirectLight light;
uniform vec3 cameraPos;
uniform bool lightenabled;
uniform float near;
uniform float far;
in vec2 texCoord;
out vec4 lightBuffer;
out vec4 glowBuffer;

void main() {
	float hyperDepth = texture2D(depthTex,texCoord).r;
	float depth = (2.0 * near) / (far + near - (hyperDepth*2.0-1.0) * (far - near));
	vec3 lightcolor;
	if (depth>0.999)
		lightcolor = vec3(1.0);

	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;
	
	float mapNum=0.0;
	float sfilter=4.0;
	mat4 shadowMat = lightMatrix[0];
	vec3 col = vec3(1.0,0.0,0.0);
	if (depth<slices[1] && depth>=slices[0]){
		shadowMat = lightMatrix[1];
		mapNum=1.0;
		sfilter=4.0;
		col = vec3(1.0,1.0,0.0);
	} else if (depth<slices[2] && depth>=slices[1]){
		shadowMat = lightMatrix[2];
		mapNum=2.0;
		sfilter=3.0;
		col = vec3(1.0,0.0,1.0);
	} else if (depth>=slices[2]) {
		shadowMat = lightMatrix[3];
		mapNum=3.0;
		sfilter=2.0;
		col = vec3(0.0,0.0,1.0);
	}
	
	vec4 shadowCoord = shadowMat*worldPos;
	
	float xOff=0.0;
	float yOff=0.0;
	float shadowij=((sfilter-0.99)/2.0);
	float pixelOff=1024.0;
	float shadow;
	for (float i=-shadowij; i<=shadowij; i++){
		for (float j=-shadowij; j<=shadowij; j++){
			xOff=i/pixelOff;
			yOff=j/pixelOff;
			if ( mapNum < 0.5 )
				shadow += textureProj(shadowMap[0], shadowCoord + vec4(xOff, yOff, -0.001, 0.0));
			else if(mapNum<1.5)
				shadow += textureProj(shadowMap[1], shadowCoord + vec4(xOff, yOff, -0.0015, 0.0));
			else if(mapNum<2.5)
				shadow += textureProj(shadowMap[2], shadowCoord + vec4(xOff, yOff, -0.002, 0.0));
			else
				shadow += textureProj(shadowMap[3], shadowCoord + vec4(xOff, yOff, -0.003, 0.0));
		}
	}
	shadow /= sfilter*sfilter;
		
	vec3 direction = normalize(light.direction);
	vec3 look = normalize(worldPos.xyz-cameraPos);
	vec3 normal = texture2D(normalTex,texCoord).xyz;
	float specular = texture2D(glowTex,texCoord).a;
	float shininess = 128.0*texture2D(normalTex,texCoord).a;
	vec3 specIntensity;
	float lightDotLook = 0.5;
	if (length(normal) < 0.1){
		lightcolor=vec3(1.0);
		specIntensity=vec3(0.0);
	} else if (!lightenabled) {
		lightcolor=vec3(0.0);
		specIntensity=vec3(0.0);
	} else {
		normal = normalize(normal*vec3(2.0)-vec3(1.0));
		lightcolor = light.color*(light.ambient+shadow*light.diffuse*clamp(dot(normal,-direction),0.0,1.0));
		vec3 reflectedLight = reflect(direction,normal);
		reflectedLight = normalize(reflectedLight);
		lightDotLook = dot(reflectedLight,-look);
		lightDotLook = clamp(lightDotLook,0.0,1.0);
		specIntensity = shadow*light.color*light.specular*pow(lightDotLook,shininess)*specular;
	}
	vec3 glow = texture2D(glowTex,texCoord).xyz;
	
	lightBuffer = vec4(lightcolor,1.0);
	glowBuffer = vec4(specIntensity+glow,1.0);
}