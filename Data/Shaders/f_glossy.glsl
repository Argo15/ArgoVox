#version 420

uniform layout ( binding = 0, r32ui ) coherent volatile uimage3D voxelmap0;
uniform layout ( binding = 1, r32ui ) coherent volatile uimage3D voxelmap1;
uniform layout ( binding = 2, r32ui ) coherent volatile uimage3D voxelmap2;
uniform layout ( binding = 3, r32ui ) coherent volatile uimage3D voxelmap3;
uniform int worldSize;
uniform int numVoxels;
uniform mat4 inverseMVPMatrix;
uniform sampler2D depthTex;
uniform sampler2D normalTex;
uniform sampler2D glowTex;
uniform vec3 cameraPos;
in vec2 texCoord;
out vec4 glossyBuffer;

float voxelWidth[4] = float[](
	float(worldSize) / float(numVoxels),
	2.0 * float(worldSize) / float(numVoxels),
	4.0 * float(worldSize) / float(numVoxels),
	8.0 * float(worldSize) / float(numVoxels)
);

vec4 convRGBA8ToVec4(uint val) {
	return vec4 (float((val&0x000000FF)), float((val&0x0000FF00)>>8U), float((val&0x00FF0000)>>16U), float((val&0xFF000000)>>24U));
}

vec4 sampleVoxelsAtWorldPos(layout ( r32ui ) coherent volatile uimage3D imgUI, vec3 worldPos, int mipFactor){
	uint xPos = uint(((worldPos.x+worldSize/2)/worldSize)*(numVoxels));
	uint yPos = uint(((worldPos.y+worldSize/2)/worldSize)*(numVoxels));
	uint zPos = uint(((worldPos.z+worldSize/2)/worldSize)*(numVoxels));
	ivec3 voxelPos = ivec3(xPos/mipFactor, yPos/mipFactor, zPos/mipFactor);
	uvec4 voxelValues = imageLoad(imgUI,voxelPos);
	return convRGBA8ToVec4(voxelValues.r);
}

vec4 voxelConeTrace(vec3 startPos, vec3 direction, vec3 normal, float coneAngle)
{
	int curMipmap = 0;
	vec4 voxelColor = vec4(0.0);
	for (float i = voxelWidth[curMipmap]*2; i < worldSize; i += voxelWidth[curMipmap])
	{
		float sliceRadius = i * tan(coneAngle);
		if (curMipmap < 3 && sliceRadius > voxelWidth[curMipmap]){
			curMipmap++;
		}
		vec3 reflection = direction * i;
		if (dot(reflection, normal) > voxelWidth[curMipmap])
		{
			if (curMipmap == 0) {
				voxelColor = sampleVoxelsAtWorldPos(voxelmap0, startPos.xyz + reflection, 1);
			} else if (curMipmap == 1) {
				voxelColor = sampleVoxelsAtWorldPos(voxelmap1, startPos.xyz + reflection, 2);
			} else if (curMipmap == 2) {
				voxelColor = sampleVoxelsAtWorldPos(voxelmap2, startPos.xyz + reflection, 4);
			} else if (curMipmap == 3) {
				voxelColor = sampleVoxelsAtWorldPos(voxelmap3, startPos.xyz + reflection, 8);
			}
			if (length(voxelColor) > 0.01)
			{
				i = worldSize;
			}
		}
	}
	return voxelColor;
}

void main() {
	vec3 normal = texture2D(normalTex,texCoord).xyz;
	if (length(normal) < 0.01)
	{
		discard;
	}
	float specular = texture2D(glowTex,texCoord).a;
	if (specular < 0.01)
	{
		discard;
	}
	float coneAngle = (1.15 - texture2D(normalTex,texCoord).a) * 3.14/8;
	
	float hyperDepth = texture2D(depthTex,texCoord).r;
	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;
	
	vec3 eyeDir = normalize(worldPos.xyz-cameraPos);
	
	normal = normalize(normal*vec3(2.0)-vec3(1.0));
	vec3 reflectedEye = reflect(eyeDir,normal);

	vec4 voxelColor = voxelConeTrace(worldPos.xyz, reflectedEye, normal, coneAngle);
	
	glossyBuffer = vec4(specular * voxelColor.rgb / vec3(255.0),1.0);
}