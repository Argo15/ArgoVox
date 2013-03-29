#version 420

uniform sampler3D voxelmap[4];
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

vec4 voxelConeTrace(vec3 startPos, vec3 direction, vec3 normal, float coneAngle)
{
	int curMipmap = 0;
	vec4 voxelColor = vec4(0.0);
	for (float i = voxelWidth[curMipmap]*2; i < worldSize; i += voxelWidth[curMipmap]/2)
	{
		float sliceRadius = i * tan(coneAngle);
		vec3 reflection = direction * i;
		if (dot(reflection, normal) > voxelWidth[curMipmap])
		{
			vec4 sampleColor = texture(voxelmap[curMipmap],((startPos+reflection+vec3(worldSize/2))/worldSize));
			float contribution = (1.0-voxelColor.a);
			voxelColor += sampleColor*contribution;
			if (voxelColor.a >= 0.99)
			{
				i = worldSize;
			}
		}
		if (curMipmap < 3 && sliceRadius > voxelWidth[curMipmap]){
			curMipmap++;
		}
	}
	if (voxelColor.a > 0)
		voxelColor.rgb /= voxelColor.a;
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
	float coneAngle = (1.25 - texture2D(normalTex,texCoord).a) * 3.14/16;
	
	float hyperDepth = texture2D(depthTex,texCoord).r;
	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;
	
	vec3 eyeDir = normalize(worldPos.xyz-cameraPos);
	
	normal = normalize(normal*vec3(2.0)-vec3(1.0));
	vec3 reflectedEye = reflect(eyeDir,normal);

	vec4 voxelColor = voxelConeTrace(worldPos.xyz, reflectedEye, normal, coneAngle);
	
	glossyBuffer = vec4(specular * voxelColor.rgb,1.0);
}