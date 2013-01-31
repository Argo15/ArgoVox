#version 420

struct DirectLight
{
  vec3 direction;
  vec3 color;
  float ambient;
  float diffuse;
};

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
uniform DirectLight light;
uniform vec3 cameraPos;
in vec4 worldPos;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
out vec4 fragColor;

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
	vec3 viewVector = normalize(vec3(worldPos.xyz/worldPos.w)-cameraPos);
	vec3 lightDir = normalize(light.direction);
	vec4 texcolor = texture2D(tex,texCoord);
	
	vec3 look = normalize(worldPos.xyz-cameraPos);
	vec3 reflectedLight = reflect(lightDir,_normal);
	reflectedLight = normalize(reflectedLight);
	float lightDotLook = dot(reflectedLight,-look);
	lightDotLook = clamp(lightDotLook,0.0,1.0);
	float spec = pow(lightDotLook,material.shininess*128.0)*material.specular;
	
	
	vec4 lightcolor = clamp(vec4(light.color,1.0) * (light.ambient+light.diffuse*clamp(dot(_normal,-lightDir),0.0,1.0)),0.0,1.0) + spec + vec4(material.emission,1.0);
	fragColor = vec4(material.color,1.0)*lightcolor*texcolor + spec;
} 