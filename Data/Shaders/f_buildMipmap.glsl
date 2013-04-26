#version 420

layout ( binding = 0, rgba8 ) uniform image2D voxelmapin;
layout ( binding = 1, rgba16 ) uniform image2D voxelPosMapIn;
layout ( binding = 2, rgba8 ) uniform image2D voxelmapout;
layout ( binding = 3, rgba16 ) uniform image2D voxelPosMapOut;
uniform int numVoxels;
uniform int hashmapSize;
uniform int hashmapPrime;
in vec2 texCoord;

ivec2 hashPos3D(ivec3 voxelPos, int offset)
{
	// int hash value
	int i = (voxelPos.x*numVoxels*numVoxels + voxelPos.y*numVoxels + voxelPos.z + offset) % hashmapPrime;
	// 2D hash position
	return ivec2(mod(i, hashmapSize), i/hashmapSize);
}

void writeVoxelColor(ivec3 voxelPos, vec4 fragmentColor)
{	
	int maxAttempts = 10;
	int offset = 0;
	for (int i=0; i<maxAttempts; i++)
	{
		offset += i;
		ivec2 hashedPos = hashPos3D(voxelPos, offset);
		vec4 savedPosColor = imageLoad(voxelPosMapOut,hashedPos);
		vec4 posColor = vec4(0,0,0,1.0);
		posColor.x = float(voxelPos.x)/float(numVoxels);
		posColor.y = float(voxelPos.y)/float(numVoxels);
		posColor.z = float(voxelPos.z)/float(numVoxels);
		if (length(savedPosColor.rgb) < 0.001)
		{
			imageStore(voxelPosMapOut,hashedPos,posColor);
			imageStore(voxelmapout,hashedPos,vec4(fragmentColor.rgb, 1.0));
			return;
		}
	}
}

vec4 readVoxelColor(ivec3 voxelPos)
{	
	vec4 voxelColor = vec4(0);
	
	int maxAttempts = 10;
	int offset = 0;
	for (int i=0; i<maxAttempts; i++)
	{
		offset += i;
		ivec2 hashedPos = hashPos3D(voxelPos, offset);
		vec4 savedPosColor = imageLoad(voxelPosMapIn,hashedPos);
		vec4 posColor = vec4(0,0,0,1.0);
		posColor.x = float(voxelPos.x)/float(numVoxels);
		posColor.y = float(voxelPos.y)/float(numVoxels);
		posColor.z = float(voxelPos.z)/float(numVoxels);
		if (length(savedPosColor.rgb) < 0.001)
		{
			return vec4(0);
		}
		if (distance(savedPosColor.rgb, posColor.rgb) < 0.0001)
		{
			voxelColor = imageLoad(voxelmapin,hashedPos);
			return voxelColor;
		}
	}
	
	return voxelColor;
}

void main() {
	int xPos = int(texCoord.x*numVoxels);
	int yPos = int(texCoord.y*numVoxels);
	for (int zPos = 0; zPos < numVoxels; zPos++)
	{
		vec4 voxelColor = vec4(0.0);
		voxelColor += readVoxelColor(ivec3(xPos*2,   yPos*2,   zPos*2));
		voxelColor += readVoxelColor(ivec3(xPos*2,   yPos*2,   zPos*2+1));
		voxelColor += readVoxelColor(ivec3(xPos*2,   yPos*2+1, zPos*2));
		voxelColor += readVoxelColor(ivec3(xPos*2,   yPos*2+1, zPos*2+1));
		voxelColor += readVoxelColor(ivec3(xPos*2+1, yPos*2,   zPos*2));
		voxelColor += readVoxelColor(ivec3(xPos*2+1, yPos*2,   zPos*2+1));
		voxelColor += readVoxelColor(ivec3(xPos*2+1, yPos*2+1, zPos*2));
		voxelColor += readVoxelColor(ivec3(xPos*2+1, yPos*2+1, zPos*2+1));
		voxelColor = voxelColor / vec4(8.0);
		if (voxelColor.a > 0.01)
		{
			voxelColor /= voxelColor.a;
			ivec3 voxelPos = ivec3(xPos, yPos, zPos);
			writeVoxelColor(voxelPos,voxelColor);
		}
	}
} 