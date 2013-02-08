#ifndef GBUFFER_H
#define GBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <vector>
#include "Chunk.h"
#include "WorldState.h"
#include "Camera.h"
#include "Frustum.h"
#include "View.h"
#include "Grid.h"
using namespace std;

class GBuffer 
{
private:
	GLuint m_nDepthTex;
	GLuint m_nNormalTex;
	GLuint m_nColorTex;
	GLuint m_nGlowTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	GBuffer(int nWidth, int nHeight);
	~GBuffer();

	void drawToBuffer(View *view, Camera *camera, Grid *myGrid);

	void bind();
	void unbind();

	void bindDepthTex();
	void bindNormalTex();
	void bindColorTex();
	void bindGlowTex();

	GLuint getGlowTex();
	GLuint getDepthTex();
	GLuint getColorTex();
	GLuint getNormalTex();

	int getWidth();
	int getHeight();
};

#endif