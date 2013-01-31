#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

#include <windows.h>
#include <gl/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Renderer.h"
#include "View.h"
#include "WorldCamera.h"
#include "Frustum.h"
#include "GBuffer.h"
#include "LightBuffer.h"
#include "FinalBuffer.h"
#include "MotionBlurBuffer.h"
#include "AtmosphereBuffer.h"

class GBuffer;
class LightBuffer;
class FinalBuffer;
class MotionBlurBuffer;
class AtmosphereBuffer;

class WorldRenderer : public Renderer
{
private:
	View *m_view;
	WorldCamera *m_camera;
	Frustum *m_frustum;
	GBuffer *m_gBuffer;
	LightBuffer *m_lightBuffer;
	FinalBuffer *m_finalBuffer;
	MotionBlurBuffer *m_motionBlurBuffer;
	AtmosphereBuffer *m_atmosphereBuffer;

	void forwardRender();
	void defferedRender();

public:
	WorldRenderer();

	void init();
	void resize(int nWidth, int nHeight);
	void render();

	View *getView();
	Frustum *getFrustum();
};

#endif