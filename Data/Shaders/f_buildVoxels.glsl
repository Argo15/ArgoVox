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
layout ( binding = 0, rgba8 ) uniform image2D voxelmap;
layout ( binding = 1, rgba16 ) uniform image2D voxelPosMap;
uniform Material material;
uniform DirectLight light;
uniform sampler2DShadow shadowMap;
uniform mat4 lightMatrix;
uniform int worldSize;
uniform int numVoxels;
uniform int hashmapSize;
uniform int hashmapPrime;

in VertexData {
	vec3 worldPos;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec2 texCoord;
} VertexIn;

out vec4 fragColor;

ivec2 hashPos3D(ivec3 voxelPos, int offset)
{
	// int hash value
	int i = (voxelPos.x*numVoxels*numVoxels + voxelPos.y*numVoxels + voxelPos.z + offset) % hashmapPrime;
	// 2D hash position
	return ivec2(mod(i, hashmapSize), i/hashmapSize);
}

void storeIfBrighter(ivec2 hashedPos, vec4 colorToStore)
{
	vec4 voxelColor = imageLoad(voxelmap,hashedPos);
	float storeIllum = colorToStore.r*0.1+colorToStore.g*0.6+colorToStore.b*0.3;
	float voxelIllum = voxelColor.r*0.1+voxelColor.g*0.6+voxelColor.b*0.3;
	if (storeIllum > voxelIllum)
	{
		imageStore(voxelmap,hashedPos,colorToStore);
	}
}

void writeVoxelColor(vec3 worldPos, vec4 fragmentColor)
{
	uint xPos = uint(((worldPos.x+worldSize/2)/worldSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+worldSize/2)/worldSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+worldSize/2)/worldSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	
	int maxAttempts = 10;
	int offset = 0;
	for (int i=0; i<maxAttempts; i++)
	{
		offset += i;
		ivec2 hashedPos = hashPos3D(voxelPos, offset);
		vec4 savedPosColor = imageLoad(voxelPosMap,hashedPos);
		vec4 posColor = vec4(0,0,0,1.0);
		posColor.x = float(xPos)/float(numVoxels);
		posColor.y = float(yPos)/float(numVoxels);
		posColor.z = float(zPos)/float(numVoxels);
		if (length(savedPosColor.rgb) < 0.001)
		{
			imageStore(voxelPosMap,hashedPos,posColor);
			imageStore(voxelmap,hashedPos,vec4(fragmentColor.rgb, 1.0));
			return;
		}
		if (distance(savedPosColor.rgb, posColor.rgb) < 0.0001)
		{
			storeIfBrighter(hashedPos, vec4(fragmentColor.rgb, 1.0));
			return;
		}
	}
}

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
	
	vec4 fragmentColor = clamp(lightcolor * vec4(material.color,1.0) * vec4(texcolor.rgb,1.0), vec4(0.0), vec4(1.0));
	fragmentColor += vec4(material.emission,1.0);
	fragmentColor = clamp(fragmentColor, vec4(0.0), vec4(1.0));
	
	writeVoxelColor(VertexIn.worldPos, fragmentColor);

	fragColor = vec4(material.shininess)+vec4(material.specular)+fragmentColor+vec4(_normal, 1.0)+vec4(1.0);
} 