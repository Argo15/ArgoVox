#version 420

layout ( binding = 0, rgba8 ) uniform image2D voxelmap;
layout ( binding = 1, rgba16 ) uniform image2D voxelPosMap;
uniform int worldSize;
uniform int numVoxels;
uniform int mipLevel;
in vec4 worldPos;
out vec4 fragColor;

ivec2 hashPos3D(ivec3 voxelPos)
{
	return ivec2(
		mod(voxelPos.x + voxelPos.z, numVoxels), 
		mod(voxelPos.y + voxelPos.z, numVoxels)
	);
}

ivec3 getSavedPos(ivec2 hashedPos)
{
	vec3 posColor = imageLoad(voxelPosMap,hashedPos).xyz;
	ivec3 savedPos = ivec3(posColor.x*numVoxels, posColor.y*numVoxels, posColor.z*numVoxels);
	return savedPos;
}

ivec2 incrementHashPos(ivec2 hashedPos, int amt)
{
	hashedPos.x += amt;
	if (hashedPos.x >= numVoxels)
	{
		hashedPos.y++;
		hashedPos.x -= numVoxels;
	}
	if (hashedPos.y >= numVoxels)
	{
		hashedPos.y -= numVoxels;
	}
	return hashedPos;
}

vec4 readVoxelColor(vec3 worldPos)
{
	uint xPos = uint(((worldPos.x+worldSize/2)/worldSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+worldSize/2)/worldSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+worldSize/2)/worldSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos, yPos, zPos);
	ivec2 hashedPos = hashPos3D(voxelPos);
	vec4 voxelColor = vec4(0);
	
	int maxAttempts = 10;
	
	for (int i=0; i<maxAttempts; i++)
	{
		hashedPos = incrementHashPos(hashedPos, i);
		vec4 savedPosColor = imageLoad(voxelPosMap,hashedPos);
		vec4 posColor = vec4(0,0,0,1.0);
		posColor.x = float(xPos)/float(numVoxels);
		posColor.y = float(yPos)/float(numVoxels);
		posColor.z = float(zPos)/float(numVoxels);
		if (distance(savedPosColor.rgb, posColor.rgb) < 0.01)
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