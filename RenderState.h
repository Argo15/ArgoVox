#ifndef RENDER_STATE_H
#define RENDER_STATE_H

enum RenderState
{
	FORWARD, 
	FINAL, 
	COLOR,  
	NORMALMAP, 
	LIGHTING, 
	INDIRECT,
	REFLECTION,
	SPECULAR,
	VOXELMAP, 
	VOXEL
};

class RenderStateManager
{
public:
	static RenderState RENDERSTATE;
};

#endif