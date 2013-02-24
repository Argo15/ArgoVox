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
layout ( binding = 0, rgba8 ) uniform image3D voxelmap0;
layout ( binding = 1, rgba8 ) uniform image3D voxelmap1;
layout ( binding = 2, rgba8 ) uniform image3D voxelmap2;
layout ( binding = 3, rgba8 ) uniform image3D voxelmap3;
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
	
	imageStore(voxelmap0,voxelPos/ivec3(1),vec4(fragmentColor.rgb, 1.0));
	imageStore(voxelmap1,voxelPos/ivec3(2),vec4(fragmentColor.rgb, 1.0));
	imageStore(voxelmap2,voxelPos/ivec3(4),vec4(fragmentColor.rgb, 1.0));
	imageStore(voxelmap3,voxelPos/ivec3(8),vec4(fragmentColor.rgb, 1.0));
	
	fragColor = vec4(material.shininess)+vec4(material.specular)+fragmentColor+vec4(_normal, 1.0)+vec4(1.0);
} 