#ifndef RENDER_STATE_H
#define RENDER_STATE_H

enum RenderState
{
	FORWARD, 
	FINAL, 
	COLOR, 
	POSITION, 
	NORMALMAP, 
	LIGHTING, 
	SSAO, 
	SPECULAR,
	MOTION,
	VOXEL
};

class RenderStateManager
{
public:
	static RenderState RENDERSTATE;
};

#endif