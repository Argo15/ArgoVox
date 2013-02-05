#version 420

struct Material
{
	bool normalenabled;
	vec3 color;
	vec3 emission;
	float specular;
	float shininess;
};

uniform sampler2D tex;
uniform sampler2D normalmap;
uniform layout ( binding = 3, r32ui ) coherent volatile uimage3D voxelmap;
uniform Material material;
in vec3 worldPos;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 colorBuffer;
out vec4 glowBuffer;

uint voxelGridSize = 16;
uint numVoxels = 32;

vec4 convRGBA8ToVec4(uint val) {
	return vec4 (float((val&0x000000FF))/255.0f, float((val&0x0000FF00)>>8U)/255.0f, float((val&0x00FF0000)>>16U)/255.0f, float((val&0xFF000000)>>24U)/255.0f);
}

uint convVec4ToRGBA8(vec4 val) {
	return (uint(val.w)&0x000000FF)<<24U | (uint(val.z)&0x000000FF)<<16U | (uint(val.y)&0x000000FF)<<8U | (uint(val.x)&0x000000FF);
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
		normalcolor = vec3(0,0,0);
	normalcolor = normalcolor*2.0-vec3(1.0);
	vec3 _normal = normalize(tangmat*normalcolor);
	vec4 texcolor = texture2D(tex,texCoord);
	
	// get voxel color
	uint xPos = uint(((worldPos.x+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	uvec4 voxelValues = imageLoad(voxelmap,voxelPos);
	vec4 voxelColor = convRGBA8ToVec4(voxelValues.r);
	
	colorBuffer = vec4(material.color,1.0)*vec4(texcolor.rgb,1.0)*0.001+vec4(voxelColor.rgb,1.0);
	normalBuffer = vec4(_normal*vec3(0.5)+vec3(0.5),material.shininess);
	glowBuffer = vec4(material.emission,material.specular);
} 