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
uniform Material material;
uniform DirectLight light;
uniform sampler2DShadow shadowMap;
uniform mat4 lightMatrix;
uniform int voxelGridSize;
uniform int numVoxels;

in VertexData {
	vec3 worldPos;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec2 texCoord;
} VertexIn;

out vec4 fragColor;

void main() {
	mat3 tangmat;
	tangmat[0] = normalize(VertexIn.tangent);
	tangmat[1] = normalize(VertexIn.bitangent);
	tangmat[2] = normalize(VertexIn.normal);
	vec3 normalcolor;
	if (material.normalenabled)
		normalcolor = texture2D(normalmap,VertexIn.texCoord).xyz;
	else 
		normalcolor = vec3(0.5,0.5,1.0);
	normalcolor = normalcolor*2.0-vec3(1.0);
	vec3 _normal = normalize(tangmat*normalcolor);
	vec4 texcolor = texture2D(tex,VertexIn.texCoord);
	
	vec4 shadowCoord = lightMatrix * vec4(VertexIn.worldPos, 1.0);
	float shadow = textureProj(shadowMap, shadowCoord + vec4(0, 0, -0.002, 0.0));
	
	vec3 lightDir = normalize(light.direction);
	vec4 lightcolor = clamp(vec4(light.color,1.0) * (light.ambient + shadow * light.diffuse*clamp(dot(_normal,-lightDir),0.0,1.0)),0.0,1.0);

	uint xPos = uint(((VertexIn.worldPos.x+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint yPos = uint(((VertexIn.worldPos.y+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	uint zPos = uint(((VertexIn.worldPos.z+voxelGridSize/2)/voxelGridSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	
	vec4 fragmentColor = clamp(lightcolor * vec4(material.color,1.0) * vec4(texcolor.rgb,1.0), vec4(0.0), vec4(1.0));
	fragmentColor += vec4(material.emission,1.0);
	fragmentColor = clamp(fragmentColor, vec4(0.0), vec4(1.0));
	
	imageStore(voxelmap0,voxelPos/ivec3(1),vec4(fragmentColor.rgb, 1.0));

	fragColor = vec4(material.shininess)+vec4(material.specular)+fragmentColor+vec4(_normal, 1.0)+vec4(1.0);
} 