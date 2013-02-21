#ifndef RENDER_STATE_H
#define RENDER_STATE_H

enum RenderState
{
	FORWARD, 
	FINAL, 
	COLOR,  
	NORMALMAP, 
	TANGENT,
	LIGHTING, 
	SPECULAR,
	REFLECTION,
	SSAO, 
	VOXEL
};

class RenderStateManager
{
public:
	static RenderState RENDERSTATE;
};

#endif