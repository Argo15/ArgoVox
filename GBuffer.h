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
using namespace std;

class GBuffer 
{
private:
	GLuint m_nDepthTex;
	GLuint m_nNormalTex;
	GLuint m_nColorTex;
	GLuint m_nWorldPosTex;
	GLuint m_nGlowTex;
	GLuint m_nMotionTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;
	Camera m_lastCamera;
	glm::mat4 m_m4LastCameraProj;

public:
	GBuffer(int nWidth, int nHeight);
	~GBuffer();

	void drawToBuffer(View *view);

	void bind();
	void unbind();

	void bindDepthTex();
	void bindNormalTex();
	void bindColorTex();
	void bindPositionTex();
	void bindGlowTex();
	void bindMotionTex();

	GLuint getGlowTex();
	GLuint getDepthTex();
	GLuint getColorTex();
	GLuint getNormalTex();
	GLuint getWorldPosTex();
	GLuint getMotionTex();

	int getWidth();
	int getHeight();
};

#endif