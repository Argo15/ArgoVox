#version 420

layout ( binding = 0, rgba8 ) uniform image2D voxelmap;
layout ( binding = 1, rgba16 ) uniform image2D voxelPosMap;
uniform int worldSize;
uniform int numVoxels;
uniform int mipLevel;
uniform int hashmapSize;
uniform int hashmapPrime;
in vec4 worldPos;
out vec4 fragColor;

ivec2 hashPos3D(ivec3 voxelPos, int offset)
{
	// int hash value
	int i = (voxelPos.x*numVoxels*numVoxels + voxelPos.y*numVoxels + voxelPos.z + offset) % hashmapPrime;
	// 2D hash position
	return ivec2(mod(i, hashmapSize), i/hashmapSize);
}

vec4 readVoxelColor(vec3 worldPos)
{
	uint xPos = uint(((worldPos.x+worldSize/2)/worldSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+worldSize/2)/worldSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+worldSize/2)/worldSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	
	vec4 voxelColor = vec4(0);
	
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
			return vec4(0);
		}
		if (distance(savedPosColor.rgb, posColor.rgb) < 0.0001)
		{
			voxelColor = imageLoad(voxelmap,hashedPos);
			return voxelColor;
		}
	}
	
	return voxelColor;
}

void main() {
	//vec2 texCoord = vec2(float(voxelPos.x)/float(numVoxels*2.0), float(voxelPos.y)/float(numVoxels*2.0));
	vec4 voxelColor = readVoxelColor(worldPos.xyz);
	
	if (voxelColor.a < 0.001) 
		discard;
	
	fragColor = vec4(voxelColor.rgb, 1.0);
} 