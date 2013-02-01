#version 420

struct Material
{
	bool normalenabled;
	vec3 color;
	vec3 emission;
	float specular;
	float shininess;
};

uniform sampler2D tex;
uniform sampler2D normalmap;
uniform Material material;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 colorBuffer;
out vec4 glowBuffer;

void main() {
	mat3 tangmat;
	tangmat[0] = normalize(tangent);
	tangmat[1] = normalize(bitangent);
	tangmat[2] = normalize(normal);
	vec3 normalcolor;
	if (material.normalenabled)
		normalcolor = texture2D(normalmap,texCoord).xyz;
	else 
		normalcolor = vec3(0.5,0.5,1.0);
	normalcolor = normalcolor*2.0-vec3(1.0);
	vec3 _normal = normalize(tangmat*normalcolor);
	vec4 texcolor = texture2D(tex,texCoord);
	
	colorBuffer = vec4(material.color,1.0)*vec4(texcolor.rgb,1.0);
	normalBuffer = vec4(_normal*vec3(0.5)+vec3(0.5),material.shininess);
	glowBuffer = vec4(material.emission,material.specular);
} 