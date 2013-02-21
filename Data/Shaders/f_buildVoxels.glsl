#version 420

struct Material
{
	bool normalenabled;
	vec3 color;
	vec3 emission;
	float specular;
	float shininess;
};

struct DirectLight
{
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};

uniform sampler2D tex;
uniform sampler2D normalmap;
uniform layout ( binding = 0, r32ui ) coherent volatile uimage3D voxelmap0;
uniform layout ( binding = 1, r32ui ) coherent volatile uimage3D voxelmap1;
uniform layout ( binding = 2, r32ui ) coherent volatile uimage3D voxelmap2;
uniform layout ( binding = 3, r32ui ) coherent volatile uimage3D voxelmap3;
//uniform layout ( binding = 4, r32ui ) coherent volatile uimage3D voxelmap4;
//uniform layout ( binding = 5, r32ui ) coherent volatile uimage3D voxelmap5;
uniform Material material;
uniform DirectLight light;
uniform sampler2DShadow shadowMap;
uniform mat4 lightMatrix;
uniform int voxelGridSize;
uniform int numVoxels;
in vec3 worldPos;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
out vec4 fragColor;

vec4 convRGBA8ToVec4(uint val) {
	return vec4 (float((val&0x000000FF)), float((val&0x0000FF00)>>8U), float((val&0x00FF0000)>>16U), float((val&0xFF000000)>>24U));
}

uint convVec4ToRGBA8(vec4 val) {
	return (uint(val.w)&0x000000FF)<<24U | (uint(val.z)&0x000000FF)<<16U | (uint(val.y)&0x000000FF)<<8U | (uint(val.x)&0x000000FF);
}

void imageAtomicRGBA8Avg( layout ( r32ui ) coherent volatile uimage3D imgUI , ivec3 coords , vec4 val ) {
	val. rgb *=255.0f; // Optimise following calculations
	uint newVal = convVec4ToRGBA8( val );
	uint prevStoredVal = 0; uint curStoredVal;
	// Loop as long as destination value gets changed by other threads
	while ( ( curStoredVal = imageAtomicCompSwap( imgUI , coords , prevStoredVal , newVal )) != prevStoredVal)
	{
		prevStoredVal = curStoredVal;
		vec4 rval = convRGBA8ToVec4( curStoredVal);
		rval . xyz =( rval . xyz* rval .w) ; // Denormalize
		vec4 curValF = rval + val; // Add new value
		curValF . xyz /=( curValF .w); // Renormalize
		newVal = convVec4ToRGBA8( curValF );
	}
}

void main() {
	mat3 tangmat;
	tangmat[0] = normalize(tangent);
	tangmat[1] = normalize(bitangent);
	tangmat[2] = normalize(normal);
	vec3 normalcolor;
	if (material.normalenabled)
		normalcolor = texture2D(normalmap,texCoord).xyz;
	else 
		normalcolor = vec3(0.5,0.5,1.0);
	normalcolor = normalcolor*2.0-vec3(1.0);
	vec3 _normal = normalize(tangmat*normalcolor);
	vec4 texcolor = texture2D(tex,texCoord);
	
	vec4 shadowCoord = lightMatrix * vec4(worldPos, 1.0);
	float shadow = textureProj(shadowMap, shadowCoord + vec4(0, 0, -0.002, 0.0));
	
	vec3 lightDir = normalize(light.direction);
	vec4 lightcolor = clamp(vec4(light.color,1.0) * (light.ambient + shadow * light.diffuse*clamp(dot(_normal,-lightDir),0.0,1.0)),0.0,1.0);

	uint xPos = uint(((worldPos.x+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	
	vec4 fragmentColor = clamp(lightcolor * vec4(material.color,1.0) * vec4(texcolor.rgb,1.0), vec4(0.0), vec4(1.0));
	fragmentColor += vec4(material.emission,1.0);
	fragmentColor = clamp(fragmentColor, vec4(0.0), vec4(1.0));
	
	imageAtomicRGBA8Avg(voxelmap0,voxelPos/ivec3(1),fragmentColor);
	imageAtomicRGBA8Avg(voxelmap1,voxelPos/ivec3(2),fragmentColor);
	imageAtomicRGBA8Avg(voxelmap2,voxelPos/ivec3(4),fragmentColor);
	imageAtomicRGBA8Avg(voxelmap3,voxelPos/ivec3(8),fragmentColor);
	//imageAtomicRGBA8Avg(voxelmap4,voxelPos/ivec3(16),fragmentColor);
	//imageAtomicRGBA8Avg(voxelmap5,voxelPos/ivec3(32),fragmentColor);
	
	fragColor = vec4(material.shininess)+vec4(material.specular)+fragmentColor+vec4(_normal, 1.0)+vec4(1.0);
} 