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
	MOTION
};

class RenderStateManager
{
public:
	static RenderState RENDERSTATE;
};

#endif