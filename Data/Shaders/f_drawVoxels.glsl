#version 420

uniform sampler3D voxelmap;
uniform int worldSize;
uniform int numVoxels;
uniform int mipLevel;
in vec4 worldPos;
out vec4 fragColor;

void main() {
	//ivec3 voxelPos = ivec3(xPos/pow(2, mipLevel), yPos/pow(2, mipLevel), zPos/pow(2, mipLevel));
	vec4 voxelColor = texture(voxelmap,(worldPos.xyz+vec3(worldSize/2))/worldSize);
	
	if (voxelColor.a < 0.001) 
		discard;
	
	fragColor = vec4(voxelColor.rgb, 1.0);
} 