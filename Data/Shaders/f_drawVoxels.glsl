#version 420

layout ( binding = 0, rgba8 ) uniform image2D voxelmap;
uniform int worldSize;
uniform int numVoxels;
uniform int mipLevel;
in vec4 worldPos;
out vec4 fragColor;

float rand(vec2 n)
{
  return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

ivec2 hashPos3D(vec3 worldPos)
{
	uint xPos = uint(((worldPos.x+worldSize/2)/worldSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+worldSize/2)/worldSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+worldSize/2)/worldSize)*(numVoxels));
	return ivec2(
		mod(xPos + zPos, numVoxels*2), 
		mod(yPos + zPos, numVoxels*2)
	);
}

void main() {
	ivec2 voxelPos = hashPos3D(worldPos.xyz/worldPos.w);
	//vec2 texCoord = vec2(float(voxelPos.x)/float(numVoxels*2.0), float(voxelPos.y)/float(numVoxels*2.0));
	vec4 voxelColor = imageLoad(voxelmap,voxelPos);
	
	if (voxelColor.a < 0.001) 
		discard;
	
	fragColor = vec4(voxelColor.rgb, 1.0);
} 