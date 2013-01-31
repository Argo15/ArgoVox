#ifndef RENDER_STATE_H
#define RENDER_STATE_H

enum RenderState
{
	FORWARD, 
	FINAL, 
	POSITION, 
	NORMALMAP, 
	COLOR, 
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