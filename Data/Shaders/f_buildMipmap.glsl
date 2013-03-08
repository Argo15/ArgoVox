#version 420

layout ( binding = 0, rgba8 ) uniform image3D voxelmapin;
layout ( binding = 1, rgba8 ) uniform image3D voxelmapout;
uniform int numVoxels;
in vec2 texCoord;

void main() {
	int xPos = int(texCoord.x*numVoxels);
	int yPos = int(texCoord.y*numVoxels);
	for (int zPos = 0; zPos < numVoxels; zPos++)
	{
		vec4 voxelColor = vec4(0.0);
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2,   yPos*2,   zPos*2));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2,   yPos*2,   zPos*2+1));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2,   yPos*2+1, zPos*2));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2,   yPos*2+1, zPos*2+1));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2+1, yPos*2,   zPos*2));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2+1, yPos*2,   zPos*2+1));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2+1, yPos*2+1, zPos*2));
		voxelColor += imageLoad(voxelmapin, ivec3(xPos*2+1, yPos*2+1, zPos*2+1));
		voxelColor = voxelColor / vec4(8.0);
		if (voxelColor.a > 0.01)
		{
			voxelColor /= voxelColor.a;
			ivec3 voxelPos = ivec3(xPos, yPos, zPos);
			imageStore(voxelmapout,voxelPos,voxelColor);
		}
	}
} 