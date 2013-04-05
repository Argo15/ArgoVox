#version 420

layout(triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;
 
in VertexData {
	vec3 worldPos;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec2 texCoord;
} VertexIn[];
 
out VertexData {
	vec3 worldPos;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec2 texCoord;
} VertexOut;
 
 void main()
{
  vec3 abVec = normalize(VertexIn[1].worldPos-VertexIn[0].worldPos);
  vec3 acVec = normalize(VertexIn[2].worldPos-VertexIn[0].worldPos);
  vec3 normalVec = abs(cross(abVec, acVec));
  int direction;
  vec4 newPosition[3];
  if (normalVec.x > normalVec.y && normalVec.x > normalVec.z)
  {
	// x focused
	for (int i=0; i<3; i++)
	{
		newPosition[i] = gl_in[i].gl_Position;
		newPosition[i].x = gl_in[i].gl_Position.z;
		newPosition[i].z = gl_in[i].gl_Position.x;
	}
  }
  else if (normalVec.y > normalVec.x && normalVec.y > normalVec.z)
  {
	// y focused
	for (int i=0; i<3; i++)
	{
		newPosition[i] = gl_in[i].gl_Position;
		newPosition[i].z = gl_in[i].gl_Position.y;
		newPosition[i].y = gl_in[i].gl_Position.z;
	}
  }
  else
  {
	// z focused
	for (int i=0; i<3; i++)
	{
		newPosition[i] = gl_in[i].gl_Position;
	}
  }
  
  for(int i = 0; i < gl_in.length(); i++)
  {
     // copy attributes
    gl_Position			= newPosition[i];
    VertexOut.worldPos	= VertexIn[i].worldPos;
    VertexOut.normal	= VertexIn[i].normal;
    VertexOut.tangent	= VertexIn[i].tangent;
    VertexOut.bitangent = VertexIn[i].bitangent;
    VertexOut.texCoord	= VertexIn[i].texCoord;
 
    // done with the vertex
    EmitVertex();
  }
  EndPrimitive();
}