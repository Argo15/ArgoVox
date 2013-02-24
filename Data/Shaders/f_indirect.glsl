#version 420

uniform layout ( binding = 0, r32ui ) coherent volatile uimage3D voxelmap0;
uniform layout ( binding = 1, r32ui ) coherent volatile uimage3D voxelmap1;
uniform layout ( binding = 2, r32ui ) coherent volatile uimage3D voxelmap2;
uniform layout ( binding = 3, r32ui ) coherent volatile uimage3D voxelmap3;
uniform int worldSize;
uniform int numVoxels;
uniform mat4 inverseMVPMatrix;
uniform sampler2D depthTex;
uniform sampler2D tanTex;
uniform sampler2D bitanTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;
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

float rand(vec2 n)
{
  return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

float coneAngle = 0.339837;
vec3 coneDirections[8] = vec3[](
    vec3(-0.450264, 0.0672209, 0.890361),
    vec3(-0.964058, 0.251081, -0.0868913),
    vec3(0.784226, 0.606053, -0.133002),
    vec3(-0.153917, 0.952092, 0.264256),
    vec3(-0.34998, 0.78714, -0.507864),
    vec3(0.733463, 0.320572, 0.599388),
    vec3(0.357086, 0.545078, -0.758538),
    vec3(-0.228467, 0.183139, -0.956171)
);

void main() {
	vec3 tangent = texture2D(tanTex,texCoord).xyz;
	if (length(tangent) < 0.01)
	{
		discard;
	}
	
	float hyperDepth = texture2D(depthTex,texCoord).r;
	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;
	
	tangent = normalize(tangent*vec3(2.0)-vec3(1.0));
	vec3 bitangent = texture2D(bitanTex,texCoord).xyz;
	bitangent = normalize(bitangent*vec3(2.0)-vec3(1.0));
	vec3 normal = cross(bitangent, tangent);
	normal = normalize(normal);
	vec3 sampledNormal = texture2D(normalTex,texCoord).xyz;
	sampledNormal = normalize(sampledNormal*vec3(2.0)-vec3(1.0));
	if (dot(normal, sampledNormal) < 0)
	{
		normal *= -1.0;
	}
	
	float randAngle = 3.14 * 2 * rand(texCoord);
	mat3 randYRot = mat3(
	   cos(randAngle), 0, -sin(randAngle), // first column (not row!)
	   0,			   1, 0,			   // second column
	   sin(randAngle), 0, cos(randAngle)   // third column
	);

	mat3 tangmat;
	tangmat[0] = normalize(bitangent);
	tangmat[1] = normalize(normal);
	tangmat[2] = normalize(tangent);
	
	vec4 voxelColor = vec4(0.0);
	float coneEndArea = 2*3.14*(1-cos(coneAngle));
	float totalHemisphereArea = 2 * 3.14;
	for (int i=0; i<8; i++)
	{
		vec3 coneDir = tangmat * randYRot * coneDirections[i];
		float contribution = coneDirections[i].y * coneEndArea;
		voxelColor += voxelConeTrace(worldPos.xyz, coneDir, normal, coneAngle) * contribution;
	}
	
	voxelColor *= 0.95;
	
	vec4 diffuse = texture2D(diffuseTex,texCoord);
	
	glossyBuffer = vec4(diffuse.rgb * (voxelColor.rgb / vec3(255.0)),1.0);
}